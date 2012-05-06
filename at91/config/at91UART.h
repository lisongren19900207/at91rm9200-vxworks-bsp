
#ifndef _AT91_UART_DEFINE_HEADER 
#define _AT91_UART_DEFINE_HEADER
/*
 *	UART control declareation
 */

#define	UART0_BASE_ADDR			0xFFFC0000
#define UART1_BASE_ADDR			0xFFFC4000
#define UART2_BASE_ADDR			0xFFFC8000
#define UART3_BASE_ADDR			0xFFFCC000
#ifndef	_ASMLANGUAGE


typedef struct _UART_S
{                                 
	AT91_REG	 US_CR; 			/* Control Register                         */
	AT91_REG	 US_MR; 			/* Mode Register                            */
	AT91_REG	 US_IER; 			/* Interrupt Enable Register                */
	AT91_REG	 US_IDR; 			/* Interrupt Disable Register               */
	AT91_REG	 US_IMR; 			/* Interrupt Mask Register                  */
	AT91_REG	 US_CSR; 			/* Channel Status Register                  */
	AT91_REG	 US_RHR; 			/* Receiver Holding Register                */
	AT91_REG	 US_THR; 			/* Transmitter Holding Register             */
	AT91_REG	 US_BRGR; 			/* Baud Rate Generator Register             */
	AT91_REG	 US_RTOR; 			/* Receiver Time-out Register               */
	AT91_REG	 US_TTGR; 			/* Transmitter Time-guard Register          */
	AT91_REG	 Reserved0[5]; 		/*                                          */
	AT91_REG	 US_FIDI; 			/* FI_DI_Ratio Register                     */
	AT91_REG	 US_NER; 			/* Nb Errors Register                       */
	AT91_REG	 US_XXR; 			/* XON_XOFF Register                        */
	AT91_REG	 US_IF; 			/* IRDA_FILTER Register                     */
	AT91_REG	 Reserved1[44]; 	/*                                          */
	PDC_S		 US_PDC;
} UART_S;

typedef struct _AT91_CHAN
{
    /* must be first */
    SIO_CHAN		sio;		/*standard SIO_CHAN element */
	
    /* callbacks */
    STATUS	        (*getTxChar) ();
    STATUS	        (*putRcvChar) ();
    void *	        getTxArg;
    void *	        putRcvArg;
	
    /* register addresses */
    UART_S		*regs;		/*UART Registers*/
    UINT32		level;		/* Interrupt Level for this device*/
	UINT32		clkdiv;
	
	
    /* misc */
    UINT32		options;	/* Hardware options */
    int         intrmode;          	/* current mode (interrupt or poll) */
    int         baudRate;       /* input clock frequency */
	UINT32		errcount;
	
} AT91_CHAN;

#endif

#define N_AT91_UART_CHANNELS 4




/*
 *	for US_CR:
 */


#define UART_CR_RESET_RECV		(1<<2)
#define UART_CR_RESET_TRAN		(1<<3)
#define UART_CR_RX_ENA			(1<<4)
#define UART_CR_RX_DIS			(1<<5)
#define UART_CR_TX_ENA			(1<<6)
#define UART_CR_TX_DIS			(1<<7)
#define UART_CR_RESET_STATUS	(1<<8)
#define UART_CR_START_BREAK		(1<<9)
#define UART_CR_STOP_BREAK		(1<<10)
#define UART_CR_START_TIME_OUT	(1<<11)
#define UART_CR_SEND_ADDR		(1<<12)
#define UART_CR_RSTIT			(1<<13)
#define UART_CR_RESET_NACK		(1<<14)
#define UART_CR_RESTART_TIMEOUT	(1<<15)
#define UART_CR_DTR_ENA			(1<<16)
#define UART_CR_DTR_DIS			(1<<17)
#define UART_CR_RTS_ENA			(1<<18)
#define UART_CR_RTS_DIS			(1<<19)

/*
 *	for US_MR:
 */

#define UART_MR_UART_MODE_NORMAL		0
#define UART_MR_UART_MODE_RS485			1
#define UART_MR_UART_MODE_HWHANDS		2
#define UART_MR_UART_MODE_MODEM			3
#define UART_MR_UART_MODE_ISO07816_0	4
#define UART_MR_UART_MODE_ISO07816_1	6
#define UART_MR_UART_MODE_IRDA			8
#define UART_MR_CLK_USE_MCK				(0<<4)
#define UART_MR_CLK_USE_MCKDIV			(1<<4)
#define UART_MR_CLK_USE_SCK				(3<<4)
#define UART_MR_CHAR_5BIT				(0<<6)
#define UART_MR_CHAR_6BIT				(1<<6)
#define UART_MR_CHAR_7BIT				(2<<6)
#define UART_MR_CHAR_8BIT				(3<<6)
#define UART_MR_SYNC_MODE				(1<<8)
#define UART_MR_PARITY_EVEN				(0<<9)
#define UART_MR_PARITY_ODD				(1<<9)
#define UART_MR_PARITY_FORCE_0			(2<<9)
#define UART_MR_PARITY_FORCE_1			(3<<9)
#define UART_MR_PARITY_NONE				(4<<9)
#define UART_MR_MULTI_DROP				(6<<9)
#define UART_MR_STOP_1BIT				(0<<12)
#define UART_MR_STOP_1BIT5				(1<<12)
#define UART_MR_STOP_2BIT				(2<<12)
#define UART_MR_TEST_NORMAL_MODE		(0<<14)
#define UART_MR_TEST_EACO_MODE			(1<<14)
#define UART_MR_TEST_LOCALLOOP_NORMAL	(2<<14)
#define UART_MR_TEST_REMOTELOOP_NORMAL	(3<<14)
#define UART_MR_MOSTBIT_FIRST			(1<<16)
#define UART_MR_CHAR_9BIT				(1<<17)
#define UART_MR_CLK_OUT_ENA				(1<<18)
#define UART_MR_8X_OVER_SAMP			(1<<19)
#define UART_MR_INHB_NACK				(1<<20)
#define UART_MR_DISABLE_NACK			(1<<21)
#define UART_MR_MAX_INTERATION			(0<<22)
#define UART_MR_IRDA_FILTER_ON			(1<<28)

#define UART_IE_DR_RXRDY		(1<<0)		/*RXRDY Interrupt Enable                                  */
#define UART_IE_DR_TXRDY        (1<<1)		/*TXRDY Interrupt Enable                                  */
#define UART_IE_DR_RXBRK        (1<<2)		/*RXBRK: Receiver Break Interrupt Enable                  */
#define UART_IE_DR_ENDRX        (1<<3)		/*ENDRX: End of Receive Transfer Interrupt Enable         */
#define UART_IE_DR_ENDTX        (1<<4)		/*ENDTX: End of Transmit Interrupt Enable                 */
#define UART_IE_DR_OVRE         (1<<5)		/*OVRE: Overrun Error Interrupt Enable                    */
#define UART_IE_DR_FRAME        (1<<6)		/*FRAME: Framing Error Interrupt Enable                   */
#define UART_IE_DR_PARE         (1<<7)		/*PARE: Parity Error Interrupt Enable                     */
#define UART_IE_DR_TIMEOUT      (1<<8)		/*TIMEOUT: Time-out Interrupt Enable                      */
#define UART_IE_DR_TXEMPTY      (1<<9)		/*TXEMPTY: TXEMPTY Interrupt Enable                       */
#define UART_IE_DR_ITERATION    (1<<10)		/*ITERATION: Iteration Interrupt Enable                   */
#define UART_IE_DR_TXBUFE       (1<<11)		/*TXBUFE: Buffer Empty Interrupt Enable                   */
#define UART_IE_DR_RXBUFF       (1<<12)		/*RXBUFF: Buffer Full Interrupt Enable                    */
#define UART_IE_DR_NACK         (1<<13)		/*NACK: Non Acknowledge Interrupt Enable                  */
#define UART_IE_DR_RIIC         (1<<16)		/*RIIC: Ring Indicator Input Change Enable                */
#define UART_IE_DR_DSRIC        (1<<17)		/*DSRIC: Data Set Ready Input Change Enable               */
#define UART_IE_DR_DCDIC        (1<<18)		/*DCDIC: Data Carrier Detect Input Change Interrupt Enable*/
#define UART_IE_DR_CTSIC        (1<<19)		/*CTSIC: Clear to Send Input Change Interrupt Enable      */
 
/* UART_CSR :status register bit layout */ 
#define UART_CSR_RXRDY		  (1<<0)	
#define UART_CSR_TXRDY        (1<<1)	
#define UART_CSR_RXBRK        (1<<2)	
#define UART_CSR_ENDRX        (1<<3)	
#define UART_CSR_ENDTX        (1<<4)	
#define UART_CSR_OVRE         (1<<5)	
#define UART_CSR_FRAME        (1<<6)	
#define UART_CSR_PARE         (1<<7)	
#define UART_CSR_TIMEOUT      (1<<8)	
#define UART_CSR_TXEMPTY      (1<<9)	
#define UART_CSR_ITERATION    (1<<10)	
#define UART_CSR_TXBUFE       (1<<11)	
#define UART_CSR_RXBUFF       (1<<12)	
#define UART_CSR_NACK         (1<<13)	
#define UART_CSR_RIIC         (1<<16)	
#define UART_CSR_DSRIC        (1<<17)	
#define UART_CSR_DCDIC        (1<<18)	
#define UART_CSR_CTSIC        (1<<19)	

#define UART_CSR_RI           (1<<20)	
#define UART_CSR_DSR          (1<<21)	
#define UART_CSR_DCD          (1<<22)	
#define UART_CSR_CTS          (1<<23)	

#define UART_TANS_TIME_GUARD	1

#endif	/*_AT91_UART_DEFINE_HEADER*/


