/* romInit_C.c - ARM AT91RM9200 ROM initialization module C part */

/*
modification history
--------------------

*/

#include "vxWorks.h"
#include "sysLib.h"
#include "asm.h"
#include "regs.h"	
#include "config.h"
#include "arch/arm/mmuArmLib.h"

/*--------------------------------------------------------------------------------------*/
/* Function Name       : AT91F_InitSDRAM						                        */
/* Object              : Initialize the SDRAM						                    */
/*--------------------------------------------------------------------------------------*/
void AT91F_InitSDRAM()
{
	SDRAMC_S* pSDRAMC = (SDRAMC_S*)SDRAMC_BASE_ADDR;
	UINT32 *pSDRAM_START = (UINT32*)LOCAL_MEM_LOCAL_ADRS;
	 
	AT91_PIO_S* pPio = (AT91_PIO_S*)PIOC_BASE_ADDR;
	
	pPio->PIO_ASR = PIOC_ENABLE_D16;
	pPio->PIO_PDR = PIOC_ENABLE_D16; /* Set in Periph mode*/
	 
	/* 
	 * MEMC should already Configure   
	 *	support all connected memories (CS0 = FLASH; CS1=SDRAM)
	 */	
	
	/*Init SDRAM*/
	pSDRAMC->SDRAMC_CR = AT91RM9200_SDRAMC_CR_VAL;  /*0x2188C159*/
	pSDRAMC->SDRAMC_MR = SDRAMC_MR_PRECHG; /*0x02*/;
	*pSDRAM_START = 0;
	pSDRAMC->SDRAMC_MR = SDRAMC_MR_REFLASH /*0x04*/;	
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 
	*pSDRAM_START = 0; 

	pSDRAMC->SDRAMC_MR = SDRAMC_MR_LDMODE/*0x03*/;	
	*(pSDRAM_START + 0x80) = 0; 
	
	pSDRAMC->SDRAMC_TR= SDRAMC_TR_VALUE;/*0x2e0*/
	*pSDRAM_START = 0; 
	
	pSDRAMC->SDRAMC_MR = SDRAMC_MR_NORMAL/*0*/;
	*pSDRAM_START = 0; 
}


/*--------------------------------------------------------------------------------------*/
/* Function Name       : AT91F_SetPLL()							                        */
/* Object              : Set the PLLA to 180MHz and Master Clock to 60Mhz		        */
/*--------------------------------------------------------------------------------------*/
void AT91F_SetPLL(void)
{
	volatile int tmp = 0;
	
	/* APMC Initialization for Crystal */
	PMC_S* pPmc = (PMC_S*)PMC_BASE_ADDR;

	pPmc->PMC_IDR = 0xFFFFFFFF;
	
	/* Setup MEMC to support all connected memories (CS0 = FLASH; CS1=SDRAM) */
	( (EBI_S*)EBI_BASE_ADDR )->EBI_CSA  = EBI_CSA_VALUE;
	

	
	/*
	 *	start the main OSC
	 */
	pPmc ->CKGR_MOR = PMC_CKGR_MOR_VALUE;

	/*
	 *	wait main OSC stability
	 */
	tmp = 0;
	while(!(pPmc->PMC_SR & PMC_SR_MOSCS ) && (tmp++ < DELAY_MAIN_FREQ)) ;

	/* -Setup the PLL A */
	pPmc->CKGR_PLLAR = AT91RM9200_PLLA_VAL; 
	
	tmp = 0;
	while(!(pPmc->PMC_SR & PMC_SR_LOCKA ) && (tmp++ < DELAY_MAIN_FREQ));
	
	pPmc->CKGR_PLLBR = AT91RM9200_PLLB_VAL; 
	
	tmp = 0;
	while(!(pPmc->PMC_SR & PMC_SR_LOCKA ) && (tmp++ < DELAY_MAIN_FREQ));
	
	/* com set CS0 cs for flash */
	*((UINT32*)(SMC_BASE_ADDR + SMC_CSR0_OFFSET)) = SMC_CSR0_VALUE;
	

	/* Write in the MCKR dirty value concerning the clock selection CSS then overwrite it in a second sequence */
	pPmc ->PMC_MCKR = 0x1;
	/* Wait until the master clock is established */
	tmp = 0;
	while(!(pPmc->PMC_SR & PMC_SR_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ));
	
	/* - Commuting Master Clock from PLLB to PLLA/3 */
	pPmc ->PMC_MCKR = AT91RM9200_PMC_MCKR_VAL;
	/* Wait until the master clock is established */
	tmp = 0;
	while(!(pPmc->PMC_SR & PMC_SR_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ));

}


void lowLevelInit()
{
	AT91F_SetPLL();
	AT91F_InitSDRAM();
}



