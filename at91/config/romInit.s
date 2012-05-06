/* romInit.s - ARM AT91RM9200 ROM initialization module */


/*
modification history
--------------------
2010/4/27 this file is modified form VxWorks demo bsp integrator920t
*/

/*
DESCRIPTION
This module contains the entry code for VxWorks images that start
running from ROM, such as 'bootrom' and 'vxWorks_rom'.  The entry
point, romInit(), is the first code executed on power-up.  It performs
the minimal setup needed to call the generic C routine romStart() with
parameter BOOT_COLD.

romInit() masks interrupts in the processor and the interrupt
controller and sets the initial stack pointer (to STACK_ADRS which is
defined in configAll.h).  Other hardware and device initialisation is
performed later in the sysHwInit routine in sysLib.c.

The routine sysToMonitor() jumps to a location after the beginning of
romInit, (defined by ROM_WARM_ADRS) to perform a "warm boot".  This
entry point allows a parameter to be passed to romStart().

The routines in this module don't use the "C" frame pointer %r11@ ! or
establish a stack frame.

*/

#define	_ASMLANGUAGE
#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "regs.h"	
#include "config.h"
#include "arch/arm/mmuArmLib.h"

        .data
        .globl   VAR(copyright_wind_river)
        .long    VAR(copyright_wind_river)

/* internals */

	.globl	FUNC(romInit)			/* start of system code */
	
	.globl	VAR(sdata)				/* start of data */
        .globl  _sdata
	.globl	VAR(integratorMemSize)	/* actual memory size */


/* externals */

	.extern	FUNC(romStart)		/* system initialization routine */
	.extern	FUNC(lowLevelInit)	/* in romInit_C.c */

_sdata:
VAR_LABEL(sdata)
	.asciz	"start of data"
	.balign	4


	.text
	.balign 4

/*******************************************************************************
*
* romInit - entry point for VxWorks in ROM
*

* romInit
*     (
*     int startType	/@ only used by 2nd entry point @/
*     )

* INTERNAL
* sysToMonitor examines the ROM for the first instruction and the string
* "Copy" in the third word so if this changes, sysToMonitor must be updated.
*/

_ARM_FUNCTION(romInit)
_romInit:
cold:
	MOV	r0, #BOOT_COLD	/* fall through to warm boot entry */
warm:
	B	start

	/* copyright notice appears at beginning of ROM (in TEXT segment) */

	.ascii   "Copyright 1999-2010 ARM Limited"
	.ascii   "\nCopyright 1999-2010 www.bjut.edu.cn."
	.balign 4

start:

	/*
	 * There have been reports of problems with certain boards and
	 * certain power supplies not coming up after a power-on reset,
	 * and adding a delay at the start of romInit appears to help
	 * with this.
	 */

	TEQS	r0, #BOOT_COLD
	MOVEQ	r1, #AT91RM9200_DELAY_VALUE
	MOVNE	r1, #1

delay_loop:
	SUBS	r1, r1, #1
	BNE	delay_loop

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
	STR	r1, [r2, #AIC_IDCR_OFFSET]	/* disable all interrups */
	STR	r1, [r2, #AIC_ICCR_OFFSET]	/* clear all interrupts */

	/*
	 * Jump to the normal (higher) ROM Position. After a reset, the
	 * ROM is mapped into memory from* location zero upwards as well
	 * as in its normal position at This code could be executing in
	 * the lower position. We wish to be executing the code, still
	 * in ROM, but in its normal (higher) position before we remap
	 * the machine so that the ROM is no longer dual-mapped from zero
	 * upwards, but so that RAM appears from 0 upwards.
	 */

	LDR	pc, L$_HiPosn
HiPosn:
	

	/*
	 * We are now executing in the normal (higher, still in ROM)
	 * position in the memory map.  Remap memory to post-reset state,
	 * so that the ROM is not now dual-mapped to zero upwards, but
	 * RAM is mapped from zero, instead.
	 */

	LDR	r1, =0x55555555
	MOV	r2, #0
	STR	r1, [r2]
	
	LDR r3, [r2]
	cmp r1, r3
	bne  mem_need_remap 
	
	LDR	r1, =0xAAAAAAAA
	STR	r1, [r2]
	LDR r3, [r2]
	beq skip_mem_remap
	
	
mem_need_remap:	
	MOV	r1, #1
	LDR	r2, =MC_BASE_ADDR
	STR	r1, [r2, #MC_RCR_OFFSET]

skip_mem_remap:
	LDR sp, =ROMINIT_C_STACK_TOP

	MOV lr, pc
	LDR pc, L$_romCInit_ADDR	/* init PLL, SDRAM */
	

	/*
	 * End of DRAM initialisation.
	 *
	 * Initialize the stack pointer to just before where the
	 * uncompress code, copied from ROM to RAM, will run.
	 */

	LDR	sp, L$_STACK_ADDR
	MOV	fp, #0			/* zero frame pointer */

	/* jump to C entry point in ROM: routine - entry point + ROM base */

#if	(ARM_THUMB)
	LDR	r12, L$_rStrtInRom
	ORR	r12, r12, #1		/* force Thumb state */
	BX	r12
#else
	LDR	pc, L$_rStrtInRom
#endif	/* (ARM_THUMB) */

/******************************************************************************/

/*
 * PC-relative-addressable pointers - LDR Rn,=sym is broken
 * note "_" after "$" to stop preprocessor performing substitution
 */

	.balign	4
	

L$_HiPosn:
	.long	ROM_TEXT_ADRS + HiPosn - FUNC(romInit)

L$_rStrtInRom:
	.long	ROM_TEXT_ADRS + FUNC(romStart) - FUNC(romInit)

L$_STACK_ADDR:
	.long	STACK_ADRS

L$_romCInit_ADDR:
	.long  ROM_TEXT_ADRS + FUNC(lowLevelInit) - FUNC(romInit) 


