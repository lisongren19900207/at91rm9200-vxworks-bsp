/*
 *	debug unit register declareation
 */
#ifndef _AT91_DBGU_DEFINE_HEADER
#define _AT91_DBGU_DEFINE_HEADER



#define	DBGU_BASE_ADDR			0xFFFFF200
#ifndef	_ASMLANGUAGE

typedef struct	_DBGU_S {                                 
	AT91_REG	 DBGU_CR; 			/* Control Register   					*/
	AT91_REG	 DBGU_MR; 			/* Mode Register                        */
	AT91_REG	 DBGU_IER; 			/* Interrupt Enable Register            */
	AT91_REG	 DBGU_IDR; 			/* Interrupt Disable Register           */
	AT91_REG	 DBGU_IMR; 			/* Interrupt Mask Register              */
	AT91_REG	 DBGU_CSR; 			/* Channel Status Register              */
	AT91_REG	 DBGU_RHR; 			/* Receiver Holding Register            */
	AT91_REG	 DBGU_THR; 			/* Transmitter Holding Register         */
	AT91_REG	 DBGU_BRGR; 		/* Baud Rate Generator Register         */
	AT91_REG	 Reserved3[7]; 		/*                                      */
	AT91_REG	 DBGU_C1R; 			/* Chip ID1 Register                    */
	AT91_REG	 DBGU_C2R; 			/* Chip ID2 Register                    */
	AT91_REG	 DBGU_FNTR; 		/* Force NTRST Register                 */
	AT91_REG	 Reserved4[45]; 	/*                                      */
	PDC_S		 DBGU_PDC;
} DBGU_S;

/*-------------------------*/


typedef struct _AT91_DBGU_CHAN
{
    /* must be first */
	
   SIO_CHAN		sio;		 	/* standard SIO_CHAN element */
	
    /* callbacks */

    STATUS	        (*getTxChar) ();
    STATUS	        (*putRcvChar) ();	
	
    void *	        getTxArg;
    void *	        putRcvArg;
	
    /* register addresses */
    DBGU_S		*regs;			/*UART Registers*/
	
	
    /* misc */
    UINT32		options;		/* Hardware options */
    int         intrmode;       /* current mode (interrupt or poll) */
    int         baudRate;       /* input clock frequency */
	UINT32		errcount;
	
} AT91_DBGU_CHAN;


#endif	/* _ASMLANGUANGE */


/*
 *	for DBGU_CR:
 */


#define DBGU_CR_RESET_RECV		(1<<2)
#define DBGU_CR_RESET_TRAN		(1<<3)
#define DBGU_CR_RX_ENA			(1<<4)
#define DBGU_CR_RX_DIS			(1<<5)
#define DBGU_CR_TX_ENA			(1<<6)
#define DBGU_CR_TX_DIS			(1<<7)
#define DBGU_CR_RESET_STATUS	(1<<8)

/*
 *	for DBGU_MR:
 */

#define DBGU_MR_PARITY_EVEN				(0<<9)
#define DBGU_MR_PARITY_ODD				(1<<9)
#define DBGU_MR_PARITY_FORCE_0			(2<<9)
#define DBGU_MR_PARITY_FORCE_1			(3<<9)
#define DBGU_MR_PARITY_NONE				(4<<9)
#define DBGU_MR_TEST_NORMAL_MODE		(0<<14)
#define DBGU_MR_TEST_EACO_MODE			(1<<14)
#define DBGU_MR_TEST_LOCALLOOP_NORMAL	(2<<14)
#define DBGU_MR_TEST_REMOTELOOP_NORMAL	(3<<14)


/*
 *	for DBGU_IER and DBGU_IDR
 */
 
#define DBGU_IE_DR_RXRDY		(1<<0)		/* RXRDY Interrupt Enable                                  */
#define DBGU_IE_DR_TXRDY        (1<<1)		/* TXRDY Interrupt Enable                                  */
#define DBGU_IE_DR_ENDRX        (1<<3)		/* ENDRX: End of Receive Transfer Interrupt Enable         */
#define DBGU_IE_DR_ENDTX        (1<<4)		/* ENDTX: End of Transmit Interrupt Enable                 */
#define DBGU_IE_DR_OVRE         (1<<5)		/* OVRE: Overrun Error Interrupt Enable                    */
#define DBGU_IE_DR_FRAME        (1<<6)		/* FRAME: Framing Error Interrupt Enable                   */
#define DBGU_IE_DR_PARE         (1<<7)		/* PARE: Parity Error Interrupt Enable                     */
#define DBGU_IE_DR_TXEMPTY      (1<<9)		/* TXEMPTY: TXEMPTY Interrupt Enable                       */
#define DBGU_IE_DR_TXBUFE       (1<<11)		/* TXBUFE: Buffer Empty Interrupt Enable                   */
#define DBGU_IE_DR_RXBUFF       (1<<12)		/* RXBUFF: Buffer Full Interrupt Enable                    */
#define DBGU_IE_DR_COMMTX       (1<<30)		/* Enable COMMTX (from ARM) Interrupt					  */
#define DBGU_IE_DR_COMMRX       (1<<31)		/* Enable COMMRX (from ARM) Interrupt       				  */


#define DBGU_CSR_RXRDY		  (1<<0)		
#define DBGU_CSR_TXRDY        (1<<1)		
#define DBGU_CSR_ENDRX        (1<<3)		
#define DBGU_CSR_ENDTX        (1<<4)		
#define DBGU_CSR_OVRE         (1<<5)		
#define DBGU_CSR_FRAME        (1<<6)		
#define DBGU_CSR_PARE         (1<<7)		
#define DBGU_CSR_TXEMPTY      (1<<9)		
#define DBGU_CSR_TXBUFE       (1<<11)		
#define DBGU_CSR_RXBUFF       (1<<12)		
#define DBGU_CSR_COMMTX       (1<<30)		
#define DBGU_CSR_COMMRX       (1<<31)

#endif	/*_AT91_DBGU_DEFINE_HEADER*/

