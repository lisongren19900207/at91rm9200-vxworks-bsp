/* at91PMC.c - AT91RM9200 PMC initial routine */

/*
modification history
--------------------

*/

/*
 *	PMC initial routine
 *	when we start up, fllow RGE already initialized:
 *	CKGR_MOR
 *	CKGR_PLLA
 *	PMC_MCKR
 *	PMC_IDR
 *	this should enable base system running
 *	next we need to initialize fllowing REG:
 *	PMC_SCER
 *	PMC_SCDR
 *	PMC_SCER
 *	PMC_PCDR
 *	PMC system interrupt, PLLB is not use, reset PLLB to quite state
 */

void Init_PMC()
{
	int dummy, i;
	PMC_S* pPmc = (PMC_S*)(PMC_BASE_ADDR);

	pPmc ->PMC_SCER = PMC_SCER_STARTUP_VALUE;
	pPmc ->PMC_SCDR = PMC_SCDR_STARTUP_VALUE;
	
	pPmc ->PMC_PCER = PMC_PCER_STARTUP_VALUE;
	pPmc ->PMC_PCDR = (~(PMC_PCER_STARTUP_VALUE) );

	/*
	 *	set PLLB to quite state
	 */
	pPmc ->CKGR_PLLBR = (0x3F<<8);
	/*
	 *	delay a while for stablized
	 */
	for( i = 0; i<1000; i++ )
		dummy = pPmc ->PMC_SR;	
}
