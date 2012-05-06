/* sysALib.s - ARM AT91RM9200 system-dependent routines */

/* Copyright 1999-2001 ARM Limited */
/* Copyright 1999-2001 Wind River Systems, Inc. */

/*
modification history
--------------------
2004/10/23 this file is modified form VxWorks demo bsp integrator920t

*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.  It contains the entry code, sysInit(), for VxWorks images
that start running from RAM, such as 'vxWorks'.  These images are
loaded into memory by some external program (e.g., a boot ROM) and then
started.  The routine sysInit() must come first in the text segment.
Its job is to perform the minimal setup needed to call the generic C
routine usrInit().

sysInit() masks interrupts in the processor and the interrupt
controller and sets the initial stack pointer.  Other hardware and
device initialisation is performed later in the sysHwInit routine in
sysLib.c.

NOTE
The routines in this module don't use the "C" frame pointer %r11@ ! or
establish a stack frame.

SEE ALSO:
.I "ARM Architecture Reference Manual,"
.I "ARM 7TDMI Data Sheet,"
.I "ARM 720T Data Sheet,"
.I "ARM 740T Data Sheet,"
.I "ARM 920T Technical Reference Manual",
.I "ARM 940T Technical Reference Manual",
.I "ARM 946E-S Technical Reference Manual",
.I "ARM 966E-S Technical Reference Manual",
.I "ARM Reference Peripherals Specification,"
.I "ARM Integrator/AP User Guide",
.I "ARM Integrator/CM7TDMI User Guide",
.I "ARM Integrator/CM720T User Guide",
.I "ARM Integrator/CM740T User Guide",
.I "ARM Integrator/CM920T User Guide",
.I "ARM Integrator/CM940T User Guide",
.I "ARM Integrator/CM946E User Guide",
.I "ARM Integrator/CM9x6ES Datasheet".
*/


#define _ASMLANGUAGE
#include "vxWorks.h"
#include "asm.h"
#include "regs.h"
#include "sysLib.h"
#include "config.h"
#include "arch/arm/mmuArmLib.h"

        .data
        .globl   VAR(copyright_wind_river)
        .long    VAR(copyright_wind_river)

/* internals */
	.globl	FUNC(sysInit)		/* start of system code */
	.globl	FUNC(sysIntStackSplit)	/* routine to split interrupt stack */

/* externals */

	.extern	FUNC(usrInit)		/* system initialization routine */

	.extern	FUNC(vxSvcIntStackBase) /* base of SVC-mode interrupt stack */
	.extern	FUNC(vxSvcIntStackEnd)	/* end of SVC-mode interrupt stack */
	.extern	FUNC(vxIrqIntStackBase) /* base of IRQ-mode interrupt stack */
	.extern	FUNC(vxIrqIntStackEnd)	/* end of IRQ-mode interrupt stack */



	.text
	.balign 4

/*******************************************************************************
*
* sysInit - start after boot
*
* This routine is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up
* the stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A

* sysInit ()              /@ THIS IS NOT A CALLABLE ROUTINE @/

*/

_ARM_FUNCTION(sysInit)

#if defined(CPU_920T)

	/*
	 * Set processor and MMU to known state as follows (we may have not
	 * been entered from a reset). We must do this before setting the CPU
	 * mode as we must set PROG32/DATA32.
	 *
	 * MMU Control Register layout.
	 *
	 * bit
	 *  0 M 0 MMU disabled
	 *  1 A 0 Address alignment fault disabled, initially
	 *  2 C 0 Data cache disabled
	 *  3 W 0 Write Buffer disabled
	 *  4 P 1 PROG32
	 *  5 D 1 DATA32
	 *  6 L 1 Should Be One (Late abort on earlier CPUs)
	 *  7 B ? Endianness (1 => big)
	 *  8 S 0 System bit to zero } Modifies MMU protections, not really
	 *  9 R 1 ROM bit to one     } relevant until MMU switched on later.
	 * 10 F 0 Should Be Zero
	 * 11 Z 0 Should Be Zero (Branch prediction control on 810)
	 * 12 I 0 Instruction cache control
	 */

	/* Setup MMU Control Register */

	MOV	r1, #MMU_INIT_VALUE		/* Defined in mmuArmLib.h */

#if defined(AT91RM9200_EARLY_I_CACHE_ENABLE)
	ORR	r1, r1, #MMUCR_I_ENABLE		/* conditionally enable Icache*/
#endif

	MCR	CP_MMU, 0, r1, c1, c0, 0	/* Write to MMU CR */

	/*
	 * If MMU was on before this, then we'd better hope it was set
	 * up for flat translation or there will be problems. The next
	 * 2/3 instructions will be fetched "translated" (number depends
	 * on CPU).
	 *
	 * We would like to discard the contents of the Write-Buffer
	 * altogether, but there is no facility to do this. Failing that,
	 * we do not want any pending writes to happen at a later stage,
	 * so drain the Write-Buffer, i.e. force any pending writes to
	 * happen now.
	 */

	MOV	r1, #0				/* data SBZ */
	MCR	CP_MMU, 0, r1, c7, c10, 4	/* drain write-buffer */

	/* Flush (invalidate) both I and D caches */

	MCR	CP_MMU, 0, r1, c7, c7, 0	/* R1 = 0 from above, data SBZ*/


    /*
	 * Set Process ID Register to zero, this effectively disables
	 * the process ID remapping feature.
	 */

	MOV	r1, #0
	MCR	CP_MMU, 0, r1, c13, c0, 0
#endif /* defined(CPU_720T,740T,920T,940T,946ES) */

	/* disable interrupts in CPU and switch to SVC32 mode */
	MRS	r1, cpsr
	BIC	r1, r1, #MASK_MODE
	ORR	r1, r1, #MODE_SVC32 | I_BIT | F_BIT
	MSR	cpsr, r1

	/*
	 * CPU INTERRUPTS DISABLED
	 *
	 * disable individual interrupts in the interrupt controller
	 */

	LDR	r2, =AIC_BASE_ADDR			/* R2->interrupt controller */
	MVN	r1, #0				/* &FFFFFFFF */
	STR	r1, [r2, #AIC_IDCR_OFFSET]	/* disable all interrupts */
	STR	r1, [r2, #AIC_ICCR_OFFSET]	/* clear all interrupts */


	/* set initial stack pointer so stack grows down from start of code */

	ADR	sp, FUNC(sysInit)		/* initialise stack pointer */

	/* now call usrInit */

	MOV	fp, #0			/* initialise frame pointer */
	MOV	r0, #BOOT_WARM_AUTOBOOT	/* pass startType */

#if	(ARM_THUMB)
	LDR	r12, L$_usrInit
	BX	r12
#else
	B	FUNC(usrInit)
#endif	/* (ARM_THUMB) */

/*******************************************************************************
*
* sysIntStackSplit - split interrupt stack and set interrupt stack pointers
*
* This routine is called, via a function pointer, during kernel
* initialisation.  It splits the allocated interrupt stack into IRQ and
* SVC-mode stacks and sets the processor's IRQ stack pointer. Note that
* the pointer passed points to the bottom of the stack allocated i.e.
* highest address+1.
*
* IRQ stack needs 6 words per nested interrupt;
* SVC-mode will need a good deal more for the C interrupt handlers.
* For now, use ratio 1:7 with any excess allocated to the SVC-mode stack
* at the lowest address.
*
* Note that FIQ is not handled by VxWorks so no stack is allocated for it.
*
* The stacks and the variables that describe them look like this.
* .CS
*
*         - HIGH MEMORY -
*     ------------------------ <--- vxIrqIntStackBase (r0 on entry)
*     |                      |
*     |       IRQ-mode       |
*     |    interrupt stack   |
*     |                      |
*     ------------------------ <--{ vxIrqIntStackEnd
*     |                      |    { vxSvcIntStackBase
*     |       SVC-mode       |
*     |    interrupt stack   |
*     |                      |
*     ------------------------ <--- vxSvcIntStackEnd
*         - LOW  MEMORY -
* .CE
*
* NOTE: This routine should not be called by the user.

* void sysIntStackSplit
*     (
*     char *pBotStack   /@ pointer to bottom of interrupt stack @/
*     long size		/@ size of stack @/
*     )

*/

_ARM_FUNCTION_CALLED_FROM_C(sysIntStackSplit)

	/*
	 * r0 = base of space allocated for stacks (i.e. highest address)
	 * r1 = size of space
	 */

	SUB	r2, r0, r1			/* r2->lowest usable address */
	LDR	r3, L$_vxSvcIntStackEnd
	STR	r2, [r3]			/*  == end of SVC-mode stack */
	SUB	r2, r0, r1, ASR #3		/* leave 1/8 for IRQ */
	LDR	r3, L$_vxSvcIntStackBase
	STR	r2, [r3]

	/* now allocate IRQ stack, setting irq_sp */

	LDR	r3, L$_vxIrqIntStackEnd
	STR	r2, [r3]
	LDR	r3, L$_vxIrqIntStackBase
	STR	r0, [r3]

	MRS	r2, cpsr
	BIC	r3, r2, #MASK_MODE
	ORR	r3, r3, #MODE_IRQ32 | I_BIT	/* set irq_sp */
	MSR	cpsr, r3
	MOV	sp, r0

	/* switch back to original mode and return */

	MSR	cpsr, r2

#if	(ARM_THUMB)
	BX	lr
#else
	MOV	pc, lr
#endif	/* (ARM_THUMB) */
		
/******************************************************************************/

/*
 * PC-relative-addressable pointers - LDR Rn,=sym is broken
 * note "_" after "$" to stop preprocessor preforming substitution
 */

	.balign	4

L$_vxSvcIntStackBase:
	.long	VAR(vxSvcIntStackBase)

L$_vxSvcIntStackEnd:
	.long	VAR(vxSvcIntStackEnd)

L$_vxIrqIntStackBase:
	.long	VAR(vxIrqIntStackBase)

L$_vxIrqIntStackEnd:
	.long	VAR(vxIrqIntStackEnd)

#if	(ARM_THUMB)
L$_usrInit:
	.long	FUNC(usrInit)
#endif	/* (ARM_THUMB) */

