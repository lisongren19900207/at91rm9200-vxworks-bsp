/* config.h - ARM Integrator configuration header */

/* Copyright 1999-2001 ARM Limited */
/* Copyright 1999-2001 Wind River Systems, Inc. */

/*
modification history
--------------------
2012/4/28 this file is modified form VxWorks demo bsp integrator920t
*/

/*
This module contains the configuration parameters for the ARM Integrator BSP.
*/

#ifndef	INCconfigh
#define	INCconfigh

#ifdef __cplusplus
extern "C" {
#endif

/* BSP version/revision identification, before configAll.h */

#define BSP_VER_1_1     1       /* 1.2 is backwards compatible with 1.1 */
#define BSP_VER_1_2     1
#define BSP_VERSION	"1.2"
#define BSP_REV		"/0"	/* 0 for first revision */

#include "configAll.h"

#if defined(CPU_920T)
#  define DEFAULT_BOOT_LINE \
    "atemac(0,0) vxw:vxWorks " \
    "h=192.168.1.100 e=192.168.1.101:ffffff00 u=at91 pw=at91 tn=AT91RM9200"
#else
#error CPU type not supported
#endif  /* defined(CPU_7TDMI) */


/* Memory configuration */

#undef	LOCAL_MEM_AUTOSIZE			/* run-time memory sizing */
#define USER_RESERVED_MEM	0		/* see sysMemTop() */

/*
 * Local-to-Bus memory address constants:
 * the local memory address always appears at 0 locally;
 * it is not dual ported.
 */

#define LOCAL_MEM_LOCAL_ADRS	0x20000000	/* fixed at 512Mbyte*/
#define LOCAL_MEM_BUS_ADRS	0x20000000	/* fixed at 1Mbyte after TCM */
#define LOCAL_MEM_SIZE		0x04000000	/* 64 Mbytes */
#define LOCAL_MEM_END_ADRS	(LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE)

#undef VEC_BASE_ADRS
#define VEC_BASE_ADRS		((char*)0)

/*
 * Boot ROM is an image written into Flash. Part of the Flash can be
 * reserved for boot parameters etc. (see the Flash section below).
 *
 * The following parameters are defined here and in the Makefile.
 * They must be kept synchronized; effectively config.h depends on Makefile.
 * Any changes made here must be made in the Makefile and vice versa.
 *
 * ROM_BASE_ADRS is the base of the Flash ROM/EPROM.
 * ROM_TEXT_ADRS is the entry point of the VxWorks image
 * ROM_SIZE is the size of the part of the Flash ROM/EPROM allocated to
 *		the VxWorks image (block size - size of headers)
 *
 * Two other constants are used:
 * ROM_COPY_SIZE is the size of the part of the ROM to be copied into RAM
 * 		 (e.g. in uncompressed boot ROM)
 * ROM_SIZE_TOTAL is the size of the entire Flash ROM (used in sysPhysMemDesc)
 *
 * The values are given as literals here to make it easier to ensure
 * that they are the same as those in the Makefile.
 */

#define ROM_BASE_ADRS       0x10000000     /* base of Flash/EPROM */
#define ROM_TEXT_ADRS       ROM_BASE_ADRS  /* code start addr in ROM */
#define ROM_SIZE            0x00200000     /* size of ROM holding VxWorks */

#define ROM_COPY_SIZE       ROM_SIZE
#define ROM_SIZE_TOTAL      0x0200000	/* total size of ROM */

#define RAM_LOW_ADRS	0x20001000 
#define RAM_HIGH_ADRS	0x23E00000 


#define N_SIO_CHANNELS	2

/*
 * Flash/NVRAM memory configuration
 *
 * A block of the Flash memory (FLASH_SIZE bytes at FLASH_ADRS) is
 * reserved for non-volatile storage of data.
 *
 * See also integrator.h
 */

#undef INCLUDE_FLASH

#ifdef INCLUDE_FLASH
#define FLASH_SIZE		0x00020000	/* one 128kbyte block of Flash*/
#define NV_RAM_SIZE		0x100		/* how much we use as NVRAM */
#undef	NV_BOOT_OFFSET
#define NV_BOOT_OFFSET		0		/* bootline at start of NVRAM */
#define FLASH_NO_OVERLAY			/* do not read-modify-write all of Flash */
#define INCLUDE_FLASH_SIB_FOOTER	/* add a SIB footer to block */
#else	/* INCLUDE_FLASH */
#define NV_RAM_SIZE		NONE

#endif	/* INCLUDE_FLASH */


/* Serial port configuration */

#define INCLUDE_SERIAL
#undef	NUM_TTY
#define NUM_TTY			N_SIO_CHANNELS+1	/* N_SIO_CHANNELS USART 1 DBGU*/


/*
 * Cache/MMU configuration
 *
 * Note that when MMU is enabled, cache modes are controlled by
 * the MMU table entries in sysPhysMemDesc[], not the cache mode
 * macros defined here.
 */

#if defined(CPU_920T)

/*
 * We use the generic architecture libraries, with caches/MMUs present. A
 * call to sysHwInit0() is needed from within usrInit before
 * cacheLibInit() is called.
 */

#ifndef _ASMLANGUAGE
IMPORT void sysHwInit0 (void);
#endif
#define INCLUDE_SYS_HW_INIT_0
#define SYS_HW_INIT_0()         sysHwInit0 ()

#endif /* defined(920T) */




#if defined(CPU_920T)
/*
 * 920T/940T/946ES I-cache mode is a bit of an inappropriate concept,
 * but use this.
 * */

#undef  USER_I_CACHE_MODE
#define USER_I_CACHE_MODE       CACHE_WRITETHROUGH

/* 920T/940T/946ES has to be this. */

#undef  USER_D_CACHE_MODE
#define USER_D_CACHE_MODE       CACHE_COPYBACK

#endif /* defined(CPU_920T/940T/946ES) */




#if defined(CPU_920T)

/*
 * Include MMU BASIC and CACHE support for command line and project builds
*/ 
#define	INCLUDE_MMU_BASIC
#define INCLUDE_CACHE_SUPPORT

#endif /* defined(720T/720T_T/920T/920T_T) */

/*
 * Network driver configuration.
 *
 * De-select unused (default) network drivers selected in configAll.h.
 */

#undef	INCLUDE_ENP		/* include CMC Ethernet interface*/
#undef	INCLUDE_EX		/* include Excelan Ethernet interface */
#undef	INCLUDE_SM_NET		/* include backplane net interface */
#undef	INCLUDE_SM_SEQ_ADDR	/* shared memory network auto address setup */

/* Enhanced Network Driver (END) Support */

#define INCLUDE_END

#ifdef	INCLUDE_END
#define	INCLUDE_AT91EMAC_END
#undef  WDB_COMM_TYPE		/* WDB agent communication path is END */
#define WDB_COMM_TYPE	WDB_COMM_END

#else  /* INCLUDE_END */

#undef  WDB_COMM_TYPE		
#define WDB_COMM_TYPE        WDB_COMM_SERIAL
#undef WDB_TTY_CHANNEL
#define WDB_TTY_CHANNEL		1 /* USART 0 */
#undef WDB_TTY_BAUD
#define  WDB_TTY_BAUD           115200
#define INCLUDE_TSFS_BOOT

#endif  /* INCLUDE_END */

#undef CONSOLE_TTY 
#define CONSOLE_TTY         0 /* DBGU */
#undef CONSOLE_BAUD_RATE
#define CONSOLE_BAUD_RATE		115200

/* PCI configuration */

#undef INCLUDE_PCI


/*
 * Interrupt mode - interrupts can be in either preemptive or non-preemptive
 * mode.  For non-preemptive mode, change INT_MODE to INT_NON_PREEMPT_MODEL
 */

#define INT_MODE	INT_NON_PREEMPT_MODEL /* INT_PREEMPT_MODEL */

/*
 * Enable BSP-configurable interrupt priorities: order of servicing and
 * masking of interrupts will be determined by ambaIntLvlPriMap[] in
 * sysLib.c.  If AMBA_INT_PRIORITY_MAP is not defined, priority of
 * interrupts will be least-significant bit first.
 */

#define	AMBA_INT_PRIORITY_MAP	/* BSP-configurable interrupt priorities */


/*
 * miscellaneous definitions
 * Note: ISR_STACK_SIZE is defined here rather than in ../all/configAll.h
 * (as is more usual) because the stack size depends on the interrupt
 * structure of the BSP.
 */

#define ISR_STACK_SIZE	0x800	/* size of ISR stack, in bytes */


/* Optional timestamp support */

#undef	INCLUDE_TIMESTAMP	/* define to include timestamp driver */
#define INCLUDE_TIMESTAMP

#define INCLUDE_POSIX_TIMERS
/* Optional TrueFFS support */

#undef	INCLUDE_TFFS		/* define to include TrueFFS driver */

#ifdef INCLUDE_TFFS
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_DOSFS
#endif /* INCLUDE_TFFS */

#undef INCLUDE_DISK_UTIL
#define INCLUDE_DISK_UTIL
#define INCLUDE_DOSFS
#define INCLUDE_RAMDRV

#include "at91rm9200.h"

#define ROMSTART_BOOT_CLEAR

#undef BSP_VTS
#ifdef BSP_VTS
/***************************************************
 * Add these defines for the Validation Test Suite *
 ***************************************************/
#define INCLUDE_SHELL
#define INCLUDE_RLOGIN
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_NET_SYM_TBL
#define INCLUDE_LOADER
#define INCLUDE_SYM_TABLE

#define INCLUDE_PING
#define INCLUDE_NET_SHOW
#endif /*BSP_VTS*/

#define INCLUDE_SHELL
#define INCLUDE_SHOW_ROUTINES
#define INCLUDE_LOADER
#define INCLUDE_PING

#ifndef	INCLUDE_SHELL
#define INCLUDE_DEMO 
#endif


#undef INCLUDE_WINDML          /* define to include windML support */
#define INCLUDE_WINDVIEW





#ifdef __cplusplus
}
#endif
#endif  /* INCconfigh */

#if defined(PRJ_BUILD)
#include "prjParams.h"
#endif

