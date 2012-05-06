
#ifndef _AT91_PMC_DEFINE_HEADER
#define _AT91_PMC_DEFINE_HEADER


/*---------------------------PMC--------------------------*/     
#define	PMC_BASE_ADDR 0xFFFFFC00  
#define PMC_CKGR_PLLAR_OFFSET	(0x28)
#define PMC_IDR_OFFSET			(0x64)
#define PMC_SR_OFFSET			(0x68)
#define PMC_MCKR_OFFSET			(0x30)
#define PMC_CKGR_MOR_OFFSET		(0x20)


#define PMC_SCER_PCK	(1<<0)
#define PMC_SCER_UDP	(1<<1)
#define PMC_SCER_MCKUDP	(1<<2)
#define PMC_SCER_UHP	(1<<4)

#define PMC_SCER_PCK0	(1<<8)
#define PMC_SCER_PCK1	(1<<9)
#define PMC_SCER_PCK2	(1<<10)
#define PMC_SCER_PCK3	(1<<11)

#define PMC_SCER_STARTUP_VALUE	(PMC_SCER_PCK)
#define PMC_SCDR_STARTUP_VALUE	\
	( PMC_SCER_UDP | PMC_SCER_UHP | PMC_SCER_PCK0 \
		| PMC_SCER_PCK1 | PMC_SCER_PCK2 | PMC_SCER_PCK3 )


#define PMC_PCER_STARTUP_VALUE \
			  ( (1<<PHER_PIOA) | (1<<PHER_PIOB) | (1<<PHER_PIOC) | (1<<PHER_PIOD) \
			  | (1<<PHER_US0) | (1<<PHER_US1) | (1<<PHER_US2) | (1<<PHER_US3) \
			  | (1<<PHER_TWI) | (1<<PHER_TC0) | (1<<PHER_TC1) | (1<<PHER_TC2) \
			  | (1<<PHER_EMAC) | (1<<PHER_AICIRQ0) | (1<<PHER_AICIRQ1) )

/*
 *	default main osc is disabled, we need to turn on it first
 */
#define PMC_CKGR_MOR_OSCOUNTER	(0xFF)
#define PMC_CKGR_MOR_VALUE 	0x01 /* ( (PMC_CKGR_MOR_OSCOUNTER<<8) | 1 ) */


/*                                                               
    
	  PLLA output frequency = (Mainclock)*(PMC_PLLA_MULA+1)/PMC_PLLA_DIVA
*/
#define	PMC_PLLA_DIVA		(MAIN_CLK_DIV)
#define	PMC_PLLA_PLLACOUNT	((0x3E)<<8) /* delay before PLL	locked */
#define	PMC_PLLA_OUTA		(2<<14)	/* frequency range 150-240M*/
#define	PMC_PLLA_MULA		((MAIN_CLK_MUL-1)<<16) /* max 2047 ,for 18,432Mhz, output	freq = 179.7M  */
#define	PMC_PLLA_VALUE		0x2026BE04;/*((1<<29)|PMC_PLLA_MULA|PMC_PLLA_OUTA|PMC_PLLA_PLLACOUNT|PMC_PLLA_DIVA)	*/ 

#define PMC_SR_MOSCS	   (1)
#define	PMC_SR_LOCKA	   (0x1<<1) /* (PMC)	PLL A Status/Enable/Disable/Mask    */	
#define	PMC_SR_MCKRDY	   (0x1<<3)	/* (PMC) MCK_RDY Status/Enable/Disable/Mask */
#define	DELAY_MAIN_FREQ		1000

#define	PMC_MCKR_CSS	(0x02)	/* use PLLA as source,Fplla as process clock, Fplla/(PMC_MCKR_MDIV+1) as MASTER	clock =	60M */
#define	PMC_MCKR_MDIV	((MASTER_CLK_DIV-1)<<8)
#define	PMC_MCKR_VALUE	(PMC_MCKR_CSS|PMC_MCKR_MDIV)

#ifndef	_ASMLANGUAGE
typedef	struct _PMC_S
{
	AT91_REG	 PMC_SCER; 			/* System Clock Enable Register			*/
	AT91_REG	 PMC_SCDR; 			/* System Clock Disable Register        */
	AT91_REG	 PMC_SCSR; 			/* System Clock Status Register         */
	AT91_REG	 Reserved34[1]; 	/*                                      */
	AT91_REG	 PMC_PCER; 			/* Peripheral Clock Enable Register     */
	AT91_REG	 PMC_PCDR; 			/* Peripheral Clock Disable Register    */
	AT91_REG	 PMC_PCSR; 			/* Peripheral Clock Status Register     */
	AT91_REG	 Reserved35[1]; 	/*                                      */
	AT91_REG	 CKGR_MOR; 			/* Main Oscillator Register             */
	AT91_REG	 CKGR_MCFR; 		/* Main Clock  Frequency Register       */
	AT91_REG	 CKGR_PLLAR; 		/* PLL A Register                       */
	AT91_REG	 CKGR_PLLBR; 		/* PLL B Register                       */
	AT91_REG	 PMC_MCKR; 			/* Master Clock Register                */
	AT91_REG	 Reserved36[3]; 	/*                                      */
	AT91_REG	 PMC_PCKR[8]; 		/* Programmable Clock Register          */
	AT91_REG	 PMC_IER; 			/* Interrupt Enable Register            */
	AT91_REG	 PMC_IDR; 			/* Interrupt Disable Register           */
	AT91_REG	 PMC_SR; 			/* Status Register                      */
	AT91_REG	 PMC_IMR; 			/* Interrupt Mask Register              */
}PMC_S;
#endif

#endif /* _AT91_PMC_DEFINE_HEADER */

