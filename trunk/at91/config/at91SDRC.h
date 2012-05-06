#ifndef _AT91_SDRAMC_DEFINE_HEADER
#define _AT91_SDRAMC_DEFINE_HEADER

/*----------------------------SDRAMC-------------------*/
#define	SDRAMC_BASE_ADDR	(EBI_BASE_ADDR+0x30)


#ifndef _ASMLANGUAGE

typedef struct _SDRAMC_S {
	AT91_REG	 SDRAMC_MR; 	/* SDRAM Controller Mode Register				*/			
	AT91_REG	 SDRAMC_TR; 	/* SDRAM Controller Refresh Timer Register      */
	AT91_REG	 SDRAMC_CR; 	/* SDRAM Controller Configuration Register      */
	AT91_REG	 SDRAMC_SRR; 	/* SDRAM Controller Self Refresh Register       */
	AT91_REG	 SDRAMC_LPR; 	/* SDRAM Controller Low Power Register          */
	AT91_REG	 SDRAMC_IER; 	/* SDRAM Controller Interrupt Enable Register   */
	AT91_REG	 SDRAMC_IDR; 	/* SDRAM Controller Interrupt Disable Register  */
	AT91_REG	 SDRAMC_IMR; 	/* SDRAM Controller Interrupt Mask Register     */
	AT91_REG	 SDRAMC_ISR; 	/* SDRAM Controller Interrupt Mask Register     */
} SDRAMC_S;

#endif	/*_ASMLANGUAGE*/

#define	SDRAMC_MR_OFFSET	(0x0)
#define	SDRAMC_TR_OFFSET	(0x4)
#define	SDRAMC_CR_OFFSET	(0x8)
#define	SDRAMC_SRR_OFFSET	(0xc)
#define	SDRAMC_LPR_OFFSET	(0x10)
#define	SDRAMC_IER_OFFSET	(0x14)
#define	SDRAMC_IDR_OFFSET	(0x18)
#define	SDRAMC_IMR_OFFSET	(0x1c)
#define	SDRAMC_ISR_OFFSET	(0x20)

#define	SDRAMC_CR_NC	(0x1)		/* 9bit column*/	
#define	SDRAMC_CR_NR	((0x1)<<2)	/* 12bit	row*/
#define	SDRAMC_CR_NB	((0x1)<<4)	/* 4 bank */
#define	SDRAMC_CR_CAS	((0x2)<<5)	/* CAS latency = 2 */
#define	SDRAMC_CR_TWR	((0x2)<<7)	/* write recovery delay	2-15 */	
#define	SDRAMC_CR_TRC	((0x8)<<11)	/* Row Cycle Delay 2-15	*/
#define	SDRAMC_CR_TRP	((0x2)<<15)	/*Row Precharge	Delay*/	
#define	SDRAMC_CR_TRCD	((0x1)<<19)	/*Row to Column	Delay*/	
#define	SDRAMC_CR_TRAS	((0x1)<<23)	/*Active to Precharge Delay*/
#define	SDRAMC_CR_TXSR	((0x1)<<27)	/*Exit Self Refresh to Active Delay*/
			


#define	SDRAMC_MR_NORMAL	(0x0)
#define	SDRAMC_MR_NOP		(0x1)
#define	SDRAMC_MR_PRECHG	(0x2)
#define	SDRAMC_MR_LDMODE	(0x3)
#define	SDRAMC_MR_REFLASH	(0x4)
#define	SDRAMC_TR_VALUE		( (MASTER_CLK*13)/1000000 )	/* 12us	for reflash cycle */

#endif	/*_AT91_SDRAMC_DEFINE_HEADER*/

