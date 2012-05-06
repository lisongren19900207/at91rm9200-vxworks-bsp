/* at91PDC.c -  routines for AT91RM9200 PDC controller */

/*
modification history
--------------------

*/

 void AT91_PDC_SetNextRx (
	PDC_S * pPDC,       /*arg pointer to a PDC controller*/
	char *address,      /*arg address to the next bloc to be received*/
	UINT32 bytes)		/*arg number of bytes to be received*/
{
	pPDC->PDC_RNPR = (UINT32) address;
	pPDC->PDC_RNCR = bytes;
}

/*----------------------------------------------------------------------------
 * \fn    AT91_PDC_SetNextTx
 * \brief Set the next transmit transfer descriptor
 *----------------------------------------------------------------------------*/
 void AT91_PDC_SetNextTx (
	PDC_S * pPDC,         /*arg pointer to a PDC controller*/
	char *address,         /*arg address to the next bloc to be transmitted*/
	UINT32 bytes)     /*arg number of bytes to be transmitted*/
{
	pPDC->PDC_TNPR = (UINT32) address;
	pPDC->PDC_TNCR = bytes;
}

/*----------------------------------------------------------------------------
  * \fn    AT91_PDC_SetRx
  * \brief Set the receive transfer descriptor
  *----------------------------------------------------------------------------*/
 void AT91_PDC_SetRx (
	PDC_S * pPDC,       /* \arg pointer to a PDC controller*/
	char *address,      /* \arg address to the next bloc to be received */
	UINT32 bytes)		/* \arg number of bytes to be received	*/
{
	pPDC->PDC_RPR = (UINT32) address;
	pPDC->PDC_RCR = bytes;
}

 /*----------------------------------------------------------------------------
  * \fn    AT91_PDC_SetTx
  * \brief Set the transmit transfer descriptor
  *----------------------------------------------------------------------------*/
 void AT91_PDC_SetTx (
	PDC_S * pPDC,			/*arg pointer to a PDC controller*/
	char *address,          /*arg address to the next bloc to be transmitted*/
	UINT32 bytes)			 /*arg number of bytes to be transmitted*/
{
	pPDC->PDC_TPR = (UINT32) address;
	pPDC->PDC_TCR = bytes;
}

 /*----------------------------------------------------------------------------
  * \fn    AT91_PDC_EnableTx
  * \brief Enable transmit
  *----------------------------------------------------------------------------*/
 void AT91_PDC_EnableTx (
	PDC_S * pPDC )        /*arg pointer to a PDC controller*/
{
	pPDC->PDC_PTCR = PDC_PTCR_TXTEN;
}

 /*----------------------------------------------------------------------------
  * \fn    AT91_PDC_EnableRx
  * \brief Enable receive
  *----------------------------------------------------------------------------*/
 void AT91_PDC_EnableRx (
	PDC_S * pPDC )       
{
	pPDC->PDC_PTCR = PDC_PTCR_RXTEN;
}

 void AT91_PDC_DisableTx (
	PDC_S * pPDC )      
{
	pPDC->PDC_PTCR = PDC_PTCR_TXTDIS;
}

void AT91_PDC_DisableRx (
	PDC_S * pPDC )
{       
	pPDC->PDC_PTCR = PDC_PTCR_RXTDIS;
}

int AT91_PDC_IsTxEmpty ( 
	PDC_S * pPDC )       
{
	return !(pPDC->PDC_TCR);
}

int AT91_PDC_IsNextTxEmpty ( 
	PDC_S * pPDC )       
{
	return !(pPDC->PDC_TNCR);
}

 int AT91_PDC_IsRxEmpty ( 
	PDC_S * pPDC )       
{
	return !(pPDC->PDC_RCR);
}

 int AT91_PDC_IsNextRxEmpty ( 
	PDC_S * pPDC )       
{
	return !(pPDC->PDC_RNCR);
}

 /*----------------------------------------------------------------------------
  * \fn    AT91_PDC_Open
  * \brief Open PDC: disable TX and RX reset transfer descriptors, re-enable RX and TX
  *----------------------------------------------------------------------------*/
 void AT91_PDC_Open (
	PDC_S * pPDC)        
{
    /* Disable the RX and TX PDC transfer requests*/
	AT91_PDC_DisableRx(pPDC);
	AT91_PDC_DisableTx(pPDC);

	/*  Reset all Counter register Next buffer first*/
	AT91_PDC_SetNextTx(pPDC, (char *) 0, 0);
	AT91_PDC_SetNextRx(pPDC, (char *) 0, 0);
	AT91_PDC_SetTx(pPDC, (char *) 0, 0);
	AT91_PDC_SetRx(pPDC, (char *) 0, 0);

     /*Enable the RX and TX PDC transfer requests*/
	AT91_PDC_EnableRx(pPDC);
	AT91_PDC_EnableTx(pPDC);
}

 /*----------------------------------------------------------------------------
  * \fn    AT91_PDC_Close
  * \brief Close PDC: disable TX and RX reset transfer descriptors
  *----------------------------------------------------------------------------*/
 void AT91_PDC_Close (
	PDC_S * pPDC)       
{
     /* Disable the RX and TX PDC transfer requests*/
	AT91_PDC_DisableRx(pPDC);
	AT91_PDC_DisableTx(pPDC);

	/* Reset all Counter register Next buffer first*/
	AT91_PDC_SetNextTx(pPDC, (char *) 0, 0);
	AT91_PDC_SetNextRx(pPDC, (char *) 0, 0);
	AT91_PDC_SetTx(pPDC, (char *) 0, 0);
	AT91_PDC_SetRx(pPDC, (char *) 0, 0);

}


 UINT32 AT91_PDC_SendFrame(
	PDC_S * pPDC,
	char *pBuffer,
	UINT32 szBuffer,
	char *pNextBuffer,
	UINT32 szNextBuffer )
{
	if (AT91_PDC_IsTxEmpty(pPDC)) {
		/* Buffer and next buffer can be initialized*/
		AT91_PDC_SetTx(pPDC, pBuffer, szBuffer);
		AT91_PDC_SetNextTx(pPDC, pNextBuffer, szNextBuffer);
		return 2;
	}
	else if (AT91_PDC_IsNextTxEmpty(pPDC)) {
		/* Only one buffer can be initialized*/
		AT91_PDC_SetNextTx(pPDC, pBuffer, szBuffer);
		return 1;
	}
	else {
		/* All buffer are in use...*/
		return 0;
	}
}

 UINT32 AT91_PDC_ReceiveFrame (
	PDC_S * pPDC,
	char *pBuffer,
	UINT32 szBuffer,
	char *pNextBuffer,
	UINT32 szNextBuffer )
{
	if (AT91_PDC_IsRxEmpty(pPDC)) {
		/* Buffer and next buffer can be initialized*/
		AT91_PDC_SetRx(pPDC, pBuffer, szBuffer);
		AT91_PDC_SetNextRx(pPDC, pNextBuffer, szNextBuffer);
		return 2;
	}
	else if (AT91_PDC_IsNextRxEmpty(pPDC)) {
		/* Only one buffer can be initialized*/
		AT91_PDC_SetNextRx(pPDC, pBuffer, szBuffer);
		return 1;
	}
	else {
		/* All buffer are in use...*/
		return 0;
	}
}

 

