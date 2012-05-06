/* at91rm9200.h	- ARM Integrator header	file */


/*
modification history
--------------------
2004/10/23 this	file is	modified form VxWorks demo bsp integrator920t
*/

/*
This file contains I/O address and related constants for the ARM9200

*/

#ifndef	INCat91rm9200h
#define	INCat91rm9200h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	_ASMLANGUAGE
	typedef volatile unsigned int AT91_REG;
	typedef unsigned char BYTE;
	/*typedef UINT AT91_REG;*/
#endif

#define SLOW_RATE_CLK		32768
#define MAIN_CLK			18432000
#define MAIN_CLK_DIV		(0x04)
#define MAIN_CLK_MUL		(0x27)
#define PROCESS_CLK			(MAIN_CLK*MAIN_CLK_MUL/MAIN_CLK_DIV)
#define MASTER_CLK_DIV		(0x02)
#define MASTER_CLK			(59904000)	/*(PROCESS_CLK/MASTER_CLK_DIV) */

#define AT91RM9200_PLLA_VAL 	 0x2026BE04
#define AT91RM9200_PLLB_VAL 	 0x10483E0E
#define AT91RM9200_PMC_MCKR_VAL  0x202
#define AT91RM9200_SDRAMC_CR_VAL 0x2188C159

#define	AT91RM9200_SSRAM_SIZE 	0x4000
#define	AT91RM9200_INT_ROM_BASE 0x100000
#define	AT91RM9200_INT_ROM_SIZE 0x020000	
	
#define	AT91RM9200_DELAY_VALUE 0x1000

#define ROMINIT_C_STACK_TOP 0x3000

#define IO_CARD_BASE 0x70000000 /* Start address of i/o card */
#define IO_CARD_SIZE 0x02000000

#define PHER_AIC 		0 		/*Advanced Interrupt Controller FIQ		*/
#define PHER_SYSIRQ     1 
#define PHER_PIOA 		2 		/*Parallel I/O Controller A		        */
#define PHER_PIOB 		3 		/*Parallel I/O Controller B             */
#define PHER_PIOC 		4 		/*Parallel I/O Controller C             */
#define PHER_PIOD 		5 		/*Parallel I/O Controller D             */
#define PHER_US0 		6 		/*USART 0                               */
#define PHER_US1 		7 		/*USART 1                               */
#define PHER_US2 		8 		/*USART 2                               */
#define PHER_US3 		9 		/*USART 3                               */
#define PHER_MCI 		10		/*Multimedia Card Interface             */
#define PHER_UDP 		11		/*USB Device Port                       */
#define PHER_TWI 		12		/*Two-wire Interface                    */
#define PHER_SPI 		13		/*Serial Peripheral Interface           */
#define PHER_SSC0 		14		/*Synchronous Serial Controller 0       */
#define PHER_SSC1 		15		/*Synchronous Serial Controller 1       */
#define PHER_SSC2 		16		/*Synchronous Serial Controller 2       */
#define PHER_TC0 		17		/*Timer/Counter 0                       */
#define PHER_TC1 		18		/*Timer/Counter 1                       */
#define PHER_TC2 		19		/*Timer/Counter 2                       */
#define PHER_TC3 		20		/*Timer/Counter 3                       */
#define PHER_TC4 		21		/*Timer/Counter 4                       */
#define PHER_TC5 		22		/*Timer/Counter 5                       */
#define PHER_UHP 		23		/*USB Host Port                         */
#define PHER_EMAC  		24		/*Ethernet MAC                          */
#define PHER_AICIRQ0 	25		/*Advanced Interrupt Controller IRQ0    */
#define PHER_AICIRQ1 	26		/*Advanced Interrupt Controller IRQ1    */
#define PHER_AICIRQ2 	27		/*Advanced Interrupt Controller IRQ2    */
#define PHER_AICIRQ3 	28		/*Advanced Interrupt Controller IRQ3    */
#define PHER_AICIRQ4 	29		/*Advanced Interrupt Controller IRQ4    */
#define PHER_AICIRQ5 	30		/*Advanced Interrupt Controller IRQ5    */
#define PHER_AICIRQ6   	31



#ifndef	_ASMLANGUAGE
#include "sioLib.h"
#endif
#include "at91PDC.h"

#include "at91DBGU.h"
#include "at91EBI.h"
#include "at91EMAC.h"		
#include "at91AIC.h"
#include "at91MCI.h"		
#include "at91PIO.h"
#include "at91PMC.h"
#include "at91RTC.h"
#include "at91SDRC.h"
#include "at91SMC.h"
#include "at91SPI.h"
#include "at91SSC.h"
#include "at91TIMER.h"
#include "at91TWI.h"
#include "at91UART.h"
#include "at91UDP.h"		

#define AT91RM9200_IO_PHYS_BASE	0xFFFA0000
#define AT91RM9200_IO_SIZE		(0xFFFFFFFF - AT91RM9200_IO_PHYS_BASE + 1)

#define	USB_DEV_BASE_ADDR	0xFFFB0000
#define	MULT_CARD_BASE_ADDR	0xFFFB4000
#define	TWO_WIRE_IF_BASE_ADDR	0xFFFB8000
#define	ETH_MAC_BASE_ADDR		0xFFFBC000
#define	SER_SYNC_CONTROL_BASE_ADDR	0xFFFD0000
#define	SPI_DEV_BASE_ADDR   0xFFFE0000


#define	BUS		BUS_TYPE_NONE

#ifdef __cplusplus
}
#endif

#endif	/* INCat91rm9200h */

