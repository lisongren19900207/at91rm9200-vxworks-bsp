/* at91PIO.c - AT91RM9200 PIO interface driver */

/*
modification history
--------------------

*/

/*
 *	forward declaration first 
 */
void AT91_PIO_Init();
void AT91_PIO_CfgPeriph( AT91_PIO_S* pPio,	unsigned int periphAEnable,	unsigned int periphBEnable);
void AT91_PIO_InterruptDisable( AT91_PIO_S* pPio, unsigned int flag);
void AT91_PIO_IntDisableAll( );
void AT91_PIO_MultDrvDisable( AT91_PIO_S* pPio, unsigned int flag);
void AT91_PIO_MultDrvDisableAll( );
void AT91_PIO_CfgIOLine( AT91_PIO_S* pPio, unsigned int outputMask,	unsigned int inputMask);


void AT91_PIO_CfgPeriph(
	AT91_PIO_S* pPio,				/*arg pointer to a PIO controller*/
	unsigned int periphAEnable,		/*arg PERIPH A to enable*/
	unsigned int periphBEnable)		/*arg PERIPH B to enable*/

{
	pPio->PIO_ASR = periphAEnable;
	pPio->PIO_BSR = periphBEnable;
	pPio->PIO_PDR = (periphAEnable | periphBEnable); /* Set in Periph mode */
}

void AT91_PIO_CfgIOLine(AT91_PIO_S* pPio,				/*arg pointer to a PIO controller*/
			unsigned int outputMask,		
			unsigned int inputMask )			
{
	pPio->PIO_OER = outputMask;
	pPio->PIO_OWER = outputMask;
	pPio->PIO_OWDR = (~outputMask);
	pPio->PIO_ODR = inputMask;
	pPio->PIO_PER = (outputMask|inputMask);
}
						

void AT91_PIO_Init()
{
	int dummy; 
	AT91_PIO_MultDrvDisableAll();
	AT91_PIO_IntDisableAll();
	
	/* clear all status change in isr */
	dummy = ((AT91_PIO_S*)(PIOA_BASE_ADDR))->PIO_ISR;
	dummy = ((AT91_PIO_S*)(PIOB_BASE_ADDR))->PIO_ISR;
	dummy = ((AT91_PIO_S*)(PIOC_BASE_ADDR))->PIO_ISR;
	dummy = ((AT91_PIO_S*)(PIOD_BASE_ADDR))->PIO_ISR;

	AT91_PIO_CfgPeriph( (AT91_PIO_S*)(PIOA_BASE_ADDR), AT91_PIOA_ASR_VALUE, AT91_PIOA_BSR_VALUE );
	AT91_PIO_CfgPeriph( (AT91_PIO_S*)(PIOB_BASE_ADDR), AT91_PIOB_ASR_VALUE, AT91_PIOB_BSR_VALUE );
	AT91_PIO_CfgPeriph( (AT91_PIO_S*)(PIOC_BASE_ADDR), AT91_PIOC_ASR_VALUE, AT91_PIOC_BSR_VALUE );
	AT91_PIO_CfgPeriph( (AT91_PIO_S*)(PIOD_BASE_ADDR), AT91_PIOD_ASR_VALUE, AT91_PIOD_BSR_VALUE );

	AT91_PIO_CfgIOLine( (AT91_PIO_S*)(PIOA_BASE_ADDR), AT91_PIOA_OER_VALUE, AT91_PIOA_ODR_VALUE );
	AT91_PIO_CfgIOLine( (AT91_PIO_S*)(PIOB_BASE_ADDR), AT91_PIOB_OER_VALUE, AT91_PIOB_ODR_VALUE );
	AT91_PIO_CfgIOLine( (AT91_PIO_S*)(PIOC_BASE_ADDR), AT91_PIOC_OER_VALUE, AT91_PIOC_ODR_VALUE );
	AT91_PIO_CfgIOLine( (AT91_PIO_S*)(PIOD_BASE_ADDR), AT91_PIOD_OER_VALUE, AT91_PIOD_ODR_VALUE );
	
}

void AT91_PIO_InterruptDisable( AT91_PIO_S* pPio,   /*arg  pointer to a PIO controller*/
								unsigned int flag)	 /*arg  pio interrupt to be disabled*/
{
	pPio->PIO_IDR = flag;
}

void AT91_PIO_IntDisableAll( )
{
	AT91_PIO_InterruptDisable( (AT91_PIO_S*)(PIOA_BASE_ADDR), 0xFFFFFFFF );
	AT91_PIO_InterruptDisable( (AT91_PIO_S*)(PIOB_BASE_ADDR), 0xFFFFFFFF );
	AT91_PIO_InterruptDisable( (AT91_PIO_S*)(PIOC_BASE_ADDR), 0xFFFFFFFF );
	AT91_PIO_InterruptDisable( (AT91_PIO_S*)(PIOD_BASE_ADDR), 0xFFFFFFFF );
}

void AT91_PIO_MultDrvDisable( AT91_PIO_S* pPio,   /*arg  pointer to a PIO controller*/
							   unsigned int flag)	 /*arg  pio interrupt to be disabled*/
{
	pPio->PIO_MDDR = flag;
}


void AT91_PIO_MultDrvDisableAll( )
{
	AT91_PIO_MultDrvDisable( (AT91_PIO_S*)(PIOA_BASE_ADDR), 0xFFFFFFFF );
	AT91_PIO_MultDrvDisable( (AT91_PIO_S*)(PIOB_BASE_ADDR), 0xFFFFFFFF );
	AT91_PIO_MultDrvDisable( (AT91_PIO_S*)(PIOC_BASE_ADDR), 0xFFFFFFFF );
	AT91_PIO_MultDrvDisable( (AT91_PIO_S*)(PIOD_BASE_ADDR), 0xFFFFFFFF );
}

