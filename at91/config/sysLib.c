/* sysLib.c - ARM AT91RM9200 system-dependent routines */

/* Copyright 1999-2001 ARM Limited */
/* Copyright 1999-2001 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
2004/10/23 this file is modified form VxWorks demo bsp integrator920t

*/

/*
DESCRIPTION
This library provides board-specific routines for the ARM Integrator
Development Board BSP.

It #includes the following chip drivers:
    nullVme.c -         dummy VMEbus routines
    ambaTimer.c -       AMBA timer driver
    ambaIntrCtl.c -     AMBA interrupt controller driver
    primeCellSio.c -	PrimeCell UART driver
    pciIomapLib.c -	PCI I/O mapping support
if INCLUDE_FLASH is defined, it #includes:
    flashMem.c -        Flash memory driver
    nvRamToFlash.c -    driver to use some Flash like NVRAM
else it #includes:
    nullNvRam.c -	dummy NVRAM routines

It #includes the following BSP files:
    sysSerial.c -	serial device initialisation routines
    sysEnd.c -		END network driver support routines.
    pciIomapShow.c -	PCI Show routines

INCLUDE FILES: sysLib.h string.h intLib.h taskLib.h vxLib.h muxLib.h
	       pciIomapLib.h

SEE ALSO:
.pG "Configuration"
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

/* includes */

#include "vxWorks.h"

#include "sioLib.h"
#include "config.h"

#include "sysLib.h"
#include "string.h"
#include "intLib.h"
#include "taskLib.h"
#include "vxLib.h"
#include "muxLib.h"
#include "cacheLib.h"

#if defined(CPU_920T)
#include "arch/arm/mmuArmLib.h"
#include "private/vmLibP.h"
/*#include "dllLib.h"*/
#endif /* defined(720T/740T/920T/940T/946ES) */


/* imports */

IMPORT char end [];			    /* end of system, created by ld */
IMPORT VOIDFUNCPTR _func_armIntStackSplit;  /* ptr to fn to split stack */



#if !defined(INCLUDE_MMU) && \
    (defined(INCLUDE_CACHE_SUPPORT) || defined(INCLUDE_MMU_BASIC) || \
     defined(INCLUDE_MMU_FULL) || defined(INCLUDE_MMU_MPU))
#define INCLUDE_MMU
#endif


/* globals */

#if defined(INCLUDE_MMU)
#if defined(CPU_920T)

/*
 * The following structure describes the various different parts of the
 * memory map to be used only during initialisation by
 * vm(Base)GlobalMapInit() when INCLUDE_MMU_BASIC/FULL are
 * defined.
 *
 * Clearly, this structure is only needed if the CPU has an MMU!
 *
 * The following are not the smallest areas that could be allocated for a
 * working system. If the amount of memory used by the page tables is
 * critical, they could be reduced.
 */

PHYS_MEM_DESC sysPhysMemDesc [] =
{
	/* adrs and length parameters must be page-aligned (multiples of 0x1000) */


	/* ram header 16K */
	{
		(void *) 0,	/* virtual address */
		(void *) 0,	/* physical address */
		ROUND_UP (AT91RM9200_SSRAM_SIZE, PAGE_SIZE), /* length, then initial state: */
		VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
		VM_STATE_VALID	| VM_STATE_WRITABLE	 | VM_STATE_CACHEABLE
	},

	/* internal rom 128K */
        {
		(void *) AT91RM9200_INT_ROM_BASE,
		(void *) AT91RM9200_INT_ROM_BASE,
		ROUND_UP (AT91RM9200_SSRAM_SIZE, PAGE_SIZE),
		VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
		VM_STATE_VALID	| VM_STATE_WRITABLE_NOT  | VM_STATE_CACHEABLE_NOT
	},
		

	/*
	 * ROM is normally marked as uncacheable by VxWorks. We leave it like that
	 * for the time being, even though this has a severe impact on execution
	 * speed from ROM.
	 */
	{
		(void *) ROM_BASE_ADRS,
		(void *) ROM_BASE_ADRS,
		ROUND_UP (ROM_SIZE_TOTAL, PAGE_SIZE),
		VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
#ifdef INCLUDE_FLASH
	/* needs to be writable */

		VM_STATE_VALID	| VM_STATE_WRITABLE	 | VM_STATE_CACHEABLE_NOT
#else
		VM_STATE_VALID	| VM_STATE_WRITABLE_NOT  | VM_STATE_CACHEABLE_NOT
#endif
	},

	/* DRAM */
	{
		(void *) LOCAL_MEM_LOCAL_ADRS,	/* virtual address */
		(void *) LOCAL_MEM_LOCAL_ADRS,	/* physical address */
		ROUND_UP (LOCAL_MEM_SIZE, PAGE_SIZE), /* length, then initial state: */
		VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
		VM_STATE_VALID	| VM_STATE_WRITABLE	 | VM_STATE_CACHEABLE
	},
	
#if     (USER_RESERVED_MEM > 0)
	/* Reserved RAM on system bus - Used for Buffer Descriptor and Buffer */
	{
		(void*)SYS_MEM_TOP,
		(void*)SYS_MEM_TOP,
		USER_RESERVED_MEM,
		VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
		VM_STATE_VALID      | VM_STATE_WRITABLE      | VM_STATE_CACHEABLE_NOT
	},
#endif  /* (USER_RESERVED_MEM > 0) */

	/* SMC for IO CARD */
	{
            (void *) IO_CARD_BASE,	/* virtual address */
            (void *) IO_CARD_BASE,	/* physical address */
            ROUND_UP (IO_CARD_SIZE, PAGE_SIZE), /* length, then initial state: */
            VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
            VM_STATE_VALID	| VM_STATE_WRITABLE	 | VM_STATE_CACHEABLE_NOT
	},

	/*
	 * I/O space:
	 * Do not map in all I/O space, only that which has something there.
	 * Otherwise we will use all of RAM allocating page tables!
	 */
	{
		(void *) AT91RM9200_IO_PHYS_BASE,	
		(void *) AT91RM9200_IO_PHYS_BASE,
		ROUND_UP (AT91RM9200_IO_SIZE, PAGE_SIZE),
		VM_STATE_MASK_VALID | VM_STATE_MASK_WRITABLE | VM_STATE_MASK_CACHEABLE,
		VM_STATE_VALID	| VM_STATE_WRITABLE	 | VM_STATE_CACHEABLE_NOT
	}
			
			
};

int sysPhysMemDescNumEnt = NELEMENTS (sysPhysMemDesc);
#endif /* defined(CPU_920T)*/

#endif /* defined(INCLUDE_MMU) */

int	sysBus	    = BUS;		/* system bus type (VME_BUS, etc) */
int	sysCpu	    = CPU;		/* system CPU type (e.g. ARMARCH4/4_T)*/
char *	sysBootLine = BOOT_LINE_ADRS; 	/* address of boot line */
char *	sysExcMsg   = EXC_MSG_ADRS;	/* catastrophic message area */
int	sysProcNum;			/* processor number of this CPU */
int	sysFlags;			/* boot flags */
char	sysBootHost [BOOT_FIELD_LEN];	/* name of host from which we booted */
char	sysBootFile [BOOT_FIELD_LEN];	/* name of file from which we booted */


/* locals */


/* defines */


/* externals */
IMPORT void At91IntDevInit (void);
IMPORT void sysIntStackSplit (char *, long);


/* globals */


/* forward LOCAL functions declarations */


/* forward declarations */

char *	sysPhysMemTop (void);


#if defined (INCLUDE_USB)
void sysUsbPciInit(void);
#endif

/* included source files */

#ifdef INCLUDE_FLASH
#include "mem/nvRamToFlash.c"
#include "flashMem.c"
#else
#include "mem/nullNvRam.c"
#endif
#include "vme/nullVme.c"

#include "at91DBGU.c"
#include "at91AIC.c"
#include "at91PDC.c"
#include "at91PIO.c"
#include "at91PMC.c"
#include "at91UART.c"
#include "at91TIMER.c"
#include "sysSerial.c"






/*
 *	before start up, we disable all devices interrupt, stop all PDC
 */

void Disable_All_Device()
{
	/*
	 *	we disable device accrod PID
	 */
	UINT32 tmp = 0xFFFFFFFF;
	UINT32 dummy;

	/*
	 *	AIC
	 */
	( (AIC_S*)AIC_BASE_ADDR ) ->AIC_IDCR = tmp;
	dummy = ( (AIC_S*)AIC_BASE_ADDR ) ->AIC_IVR;
	dummy = ( (AIC_S*)AIC_BASE_ADDR ) ->AIC_FVR;

	/*
	 *	SYS timer, DBGU, PMC..
	 */
	( (SYSTIMER_S*)SYSTIMER_BASE_ADDR ) ->ST_IDR = tmp;
	( (DBGU_S*)DBGU_BASE_ADDR ) ->DBGU_IDR = tmp;
	( (PMC_S*)PMC_BASE_ADDR ) ->PMC_IDR = tmp;
	( (RTC_S*)RTC_BASE_ADDR ) ->RTC_IDR = tmp;
	


	/*
	 *	PIO
	 */
	( (AT91_PIO_S*)PIOA_BASE_ADDR ) ->PIO_IDR = tmp;
	( (AT91_PIO_S*)PIOB_BASE_ADDR ) ->PIO_IDR = tmp;
	( (AT91_PIO_S*)PIOC_BASE_ADDR ) ->PIO_IDR = tmp;
	( (AT91_PIO_S*)PIOD_BASE_ADDR ) ->PIO_IDR = tmp;
	
	/*
	 *	UART 
	 */
	( (UART_S*)UART0_BASE_ADDR ) ->US_IDR = tmp;
	( (UART_S*)UART1_BASE_ADDR ) ->US_IDR = tmp;
	( (UART_S*)UART2_BASE_ADDR ) ->US_IDR = tmp;
	( (UART_S*)UART3_BASE_ADDR ) ->US_IDR = tmp;
	AT91_PDC_Close( &( ( (UART_S*)UART0_BASE_ADDR ) ->US_PDC ) );
	AT91_PDC_Close( &( ( (UART_S*)UART1_BASE_ADDR ) ->US_PDC ) );
	AT91_PDC_Close( &( ( (UART_S*)UART2_BASE_ADDR ) ->US_PDC ) );
	AT91_PDC_Close( &( ( (UART_S*)UART3_BASE_ADDR ) ->US_PDC ) );
		
		
	/*
	 *	MCI
	 */

	( (MCI_S*)MCI_BASE_ADDR ) ->MCI_IDR = tmp;
	AT91_PDC_Close( &( ( (MCI_S*)MCI_BASE_ADDR ) ->MCI_PDC ) );

	/*
	 *	UDP
	 */
	( (UDP_S*)UDP_BASE_ADDR ) ->UDP_IDR = tmp;

	
	/*
	 *	TWI
	 */
	( (TWI_S*)TWI_BASE_ADDR ) ->TWI_IDR = tmp;

	/*
	 *	SPI
	 */

	( (SPI_S*)SPI_BASE_ADDR ) ->SPI_IDR = tmp;
	AT91_PDC_Close( &( ( (SPI_S*)SPI_BASE_ADDR ) ->SPI_PDC ) );

	
	/*
	 *	SSC 0-2
	 */
	( (SSC_S*)SSC0_BASE_ADDR ) ->SSC_IDR = tmp;
	( (SSC_S*)SSC1_BASE_ADDR ) ->SSC_IDR = tmp;
	( (SSC_S*)SSC2_BASE_ADDR ) ->SSC_IDR = tmp;
	AT91_PDC_Close( &( ( (SSC_S*)SSC0_BASE_ADDR ) ->SSC_PDC ) );
	AT91_PDC_Close( &( ( (SSC_S*)SSC1_BASE_ADDR ) ->SSC_PDC ) );
	AT91_PDC_Close( &( ( (SSC_S*)SSC2_BASE_ADDR ) ->SSC_PDC ) );

	
	/*
	 *	TC 0 - 6
	 */
	( (TimerCounter*)TIMER0_BASE_ADDR ) ->TC_IDR = tmp;
	( (TimerCounter*)TIMER1_BASE_ADDR ) ->TC_IDR = tmp;
	( (TimerCounter*)TIMER2_BASE_ADDR ) ->TC_IDR = tmp;
	( (TimerCounter*)TIMER3_BASE_ADDR ) ->TC_IDR = tmp;
	( (TimerCounter*)TIMER4_BASE_ADDR ) ->TC_IDR = tmp;
	( (TimerCounter*)TIMER5_BASE_ADDR ) ->TC_IDR = tmp;


	/*
	 *	UHP, I don't know
	 */

	/*
	 *	EMAC
	 */

	( (EMAC_S*)EMAC_BASE_ADDR ) ->EMAC_IDR = tmp;

}

/*******************************************************************************
*
* sysModel - return the model name of the CPU board
*
* This routine returns the model name of the CPU board.
*
* NOTE
* This routine does not include all of the possible variants, and the
* inclusion of a variant in here does not mean that it is supported.
*
* RETURNS: A pointer to a string identifying the board and CPU.
*/

char *sysModel (void)
    {
#if	defined(CPU_920T)
    return	"ATMEL AT91RM9200 - ARM920T (ARM)";
#else
	#error CPU not supported
#endif	/* defined(CPU_7TDMI) */
    }

/*******************************************************************************
*
* sysBspRev - return the BSP version with the revision eg 1.2/<x>
*
* This function returns a pointer to a BSP version with the revision.
* e.g. 1.2/<x>. BSP_REV is concatenated to BSP_VERSION to form the
* BSP identification string.
*
* RETURNS: A pointer to the BSP version/revision string.
*/

char * sysBspRev (void)
    {
    return (BSP_VERSION BSP_REV);
    }

#if defined(CPU_920T)
/*******************************************************************************
*
* sysHwInit0 - perform early BSP-specific initialisation
*
* This routine performs such BSP-specific initialisation as is necessary before
* the architecture-independent cacheLibInit can be called. It is called
* from usrInit() before cacheLibInit(), before sysHwInit() and before BSS
* has been cleared.
*
* RETURNS: N/A
*/

void sysHwInit0 (void)
    {
	
	strcpy( BOOT_LINE_ADRS, DEFAULT_BOOT_LINE );

#ifdef INCLUDE_CACHE_SUPPORT
    /*
     * Install the appropriate cache library, no address translation
     * routines are required for this BSP, as the default memory map has
     * virtual and physical addresses the same.
     */

#if   defined(CPU_920T) || defined(CPU_920T_T)
    cacheArm920tLibInstall (NULL, NULL);
#endif

#endif /* INCLUDE_CACHE_SUPPORT */


#if defined(INCLUDE_MMU)
    /* Install the appropriate MMU library and translation routines */

#if   defined(CPU_920T) || defined(CPU_920T_T)
    mmuArm920tLibInstall (NULL, NULL);
#endif

#endif /* defined(INCLUDE_MMU) */

    return;
    }
#endif /* defined(720T/740T/920T/940T/946ES) */


/*******************************************************************************
*
* sysHwInit - initialize the CPU board hardware
*
* This routine initializes various features of the hardware.
* Normally, it is called from usrInit() in usrConfig.c.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*/

void sysHwInit (void)
{
    /* install the IRQ/SVC interrupt stack splitting routine */

    _func_armIntStackSplit = sysIntStackSplit;

	/* first we need to setup all the PIO config */
	Disable_All_Device();
	Init_PMC();
	AT91_PIO_Init();
	InitSystemTimer();
	TimerCounter_Global_Init();

#ifdef INCLUDE_SERIAL
    /* initialise the serial devices */
    sysSerialHwInit ();      /* initialise serial data structure */
#endif /* INCLUDE_SERIAL */
}

/*******************************************************************************
*
* sysHwInit2 - additional system configuration and initialization
*
* This routine connects system interrupts and does any additional
* configuration necessary.  Note that this is called from
* sysClkConnect() in the timer driver.
*
* RETURNS: N/A
*
*/

void sysHwInit2 (void)
{
    static BOOL initialised = FALSE;

    if (initialised)
    	return;

    /* initialise the interrupt library and interrupt driver */
    intLibInit ( AIC_INT_NUM_LEVELS, AIC_INT_NUM_LEVELS, INT_MODE );
    At91IntDevInit ();


    /* connect sys clock interrupt and auxiliary clock interrupt */
    (void)intConnect ( INUM_TO_IVEC(INT_VEC_SYSIRQ), sysClkInt, 0);
    (void)intConnect ( INUM_TO_IVEC(INT_VEC_TC0), sysAuxClkInt, 0);
	(void)intConnect ( INUM_TO_IVEC(INT_VEC_TC2), sysTimestampInt, 0 );
	InitSystemTimer2();
	intEnable ( INT_LVL_SYSIRQ );
	
#ifdef INCLUDE_SERIAL
    /* connect serial interrupt */
    sysSerialHwInit2();
#endif /* INCLUDE_SERIAL */


#if defined (INCLUDE_USB)
    /* Low level init for usb */
    sysUsbInit();

#endif


    initialised = TRUE;

}

/*******************************************************************************
*
* sysPhysMemTop - get the address of the top of physical memory
*
* This routine returns the address of the first missing byte of memory,
* which indicates the top of memory.
*
* Normally, the user specifies the amount of physical memory with the
* macro LOCAL_MEM_SIZE in config.h.  BSPs that support run-time
* memory sizing do so only if the macro LOCAL_MEM_AUTOSIZE is defined.
* If not defined, then LOCAL_MEM_SIZE is assumed to be, and must be, the
* true size of physical memory.
*
* NOTE: Do no adjust LOCAL_MEM_SIZE to reserve memory for application
* use.  See sysMemTop() for more information on reserving memory.
*
* RETURNS: The address of the top of physical memory.
*
* SEE ALSO: sysMemTop()
*/

char * sysPhysMemTop (void)
{
    static char * physTop = NULL;

    if (physTop == NULL)
	{
#ifdef LOCAL_MEM_AUTOSIZE

	/* If auto-sizing is possible, this would be the spot.  */

#error   "Dynamic memory sizing not supported"

#else
	/* Don't do autosizing, if size is given */
	physTop = (char *)(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE);

#endif /* LOCAL_MEM_AUTOSIZE */
	}

    return physTop;
}

/*******************************************************************************
*
* sysMemTop - get the address of the top of VxWorks memory
*
* This routine returns a pointer to the first byte of memory not
* controlled or used by VxWorks.
*
* The user can reserve memory space by defining the macro USER_RESERVED_MEM
* in config.h.  This routine returns the address of the reserved memory
* area.  The value of USER_RESERVED_MEM is in bytes.
*
* RETURNS: The address of the top of VxWorks memory.
*/

char * sysMemTop (void)
{
    static char * memTop = NULL;

    if (memTop == NULL)
	{
		memTop = sysPhysMemTop () - USER_RESERVED_MEM;
	}

    return memTop;
}

/*******************************************************************************
*
* sysToMonitor - transfer control to the ROM monitor
*
* This routine transfers control to the ROM monitor.  It is usually called
* only by reboot() -- which services ^X -- and bus errors at interrupt
* level.  However, in some circumstances, the user may wish to introduce a
* new <startType> to enable special boot ROM facilities.
*
* RETURNS: Does not return.
*/

STATUS sysToMonitor
    (
    int startType	/* passed to ROM to tell it how to boot */
    )
{
    FUNCPTR	pRom;
    UINT32 *	p = (UINT32 *)ROM_TEXT_ADRS;

#ifdef INCLUDE_SERIAL
    sysSerialReset ();	/* put serial devices into quiet state */
#endif

    /*
     * Examine ROM - if it's a VxWorks boot ROM, jump to the warm boot entry
     * point; otherwise jump to the start of the ROM.
     * A VxWorks boot ROM begins
     *    MOV	R0,#BOOT_COLD
     *    B	...
     *    DCB	"Copyright"
     * We check the first and third words only. This could be tightened up
     * if required (see romInit.s).
     */
    if (p[0] == 0xE3A00002 && p[2] == 0x79706F43)
		pRom = (FUNCPTR)(ROM_TEXT_ADRS + 4);	/* warm boot address */
    else
		pRom = (FUNCPTR)ROM_TEXT_ADRS;		/* start of ROM */

#if defined(CPU_920T)
    VM_ENABLE(FALSE);	/* disable the MMU, cache(s) and write-buffer */
#endif

#if defined(CPU_920T) || defined(CPU_920T_T)
    /*
     * On 920T, can have the I-cache enabled once the MMU has been
     * disabled, so, unlike the other processors, disabling the MMU does
     * not disable the I-cache.  This would not be a problem, as the
     * 920T boot ROM initialisation code disables and flushes both caches.
     * However, in case we are, in fact, using a 7TDMI boot ROM,
     * disable and flush the I-cache here, or else the boot process may
     * fail.
     */
    cacheDisable (INSTRUCTION_CACHE);
#endif /* defined(CPU_920T/920T_T) */

    (*pRom)(startType);	/* jump to boot ROM */

    return OK;		/* in case we ever continue from ROM monitor */
}

/****************************************************************************
*
* sysProcNumGet - get the processor number
*
* This routine returns the processor number for the CPU board, which is
* set with sysProcNumSet().
*
* RETURNS: The processor number for the CPU board.
*
* SEE ALSO: sysProcNumSet()
*/

int sysProcNumGet (void)
{
    return (sysProcNum);
}

/****************************************************************************
*
* sysProcNumSet - set the processor number
*
* Set the processor number for the CPU board.  Processor numbers should be
* unique on a single backplane.
*
* NOTE
* By convention, only processor 0 should dual-port its memory.
*
* RETURNS: N/A
*
* SEE ALSO: sysProcNumGet()
*/

void sysProcNumSet
	(
    int procNum		/* processor number */
    )
{
    sysProcNum = procNum;
}

/******************************************************************************
*
* sysLedsReadWrite - read/write the state of the LEDs on the board
*
* This routine can be used to read and write the state of the four LEDs
* on the board. The current state is ANDed and EORed with the supplied
* parameters. Bits 0..3 control LEDs 0..3. A 1 switches the LED on; a 0 off.
*
* RETURNS: previous state of LEDs.
*/

int sysLedsReadWrite
    (
    int and,
    int eor
    )
{
    UINT32	 	current, previous;
    int			oldLevel;

    oldLevel = intLock();

    /* read current value from hardware and mask off undefined bits */
	/*#error "LEDReadWrite not implented."*/


    intUnlock (oldLevel);
    return previous;
}

#ifdef INCLUDE_FLASH
/******************************************************************************
*
* sysFlashWriteEnable - enable write access to the Flash memory
*
* This routine is used by flashMem.c to enable write access to the
* Flash memory.
*
* RETURNS: N/A
*/

void sysFlashWriteEnable (void)
{
	return;
}

/******************************************************************************
*
* sysFlashWriteDisable - disable write access to the Flash memory
*
* This routine is used by flashMem.c to disable write access to the
* Flash memory.
*
* RETURNS: N/A
*/

void sysFlashWriteDisable (void)
     {
		return;
     }
#endif /* INCLUDE_FLASH */
