#ifndef _AT91_PIO_DEFINE_HEADER
#define _AT91_PIO_DEFINE_HEADER


/*---------------------------PIOC-------------------*/
#define PIOA_BASE_ADDR	(0xFFFFF400)
#define PIOB_BASE_ADDR	(0xFFFFF600)
#define	PIOC_BASE_ADDR	(0xFFFFF800)
#define PIOD_BASE_ADDR	(0xFFFFFA00)

#define	PIO_PDR_OFFSET	(0x04)
#define	PIO_ASR_OFFSET	(0x70)
#define	PIO_BSR_OFFSET	(0x74)

#define	PIOC_ENABLE_D16	(0xFFFF0000)

#ifndef _ASMLANGUAGE
typedef struct _AT91_PIO_S
{
	AT91_REG	 PIO_PER; 	/* PIO Enable Register			*/
	AT91_REG	 PIO_PDR; 	/* PIO Disable Register                 */
	AT91_REG	 PIO_PSR; 	/* PIO Status Register                  */
	AT91_REG	 Resv0[1]; 	/*                                      */
	AT91_REG	 PIO_OER; 	/* Output Enable Register               */
	AT91_REG	 PIO_ODR; 	/* Output Disable Registerr             */
	AT91_REG	 PIO_OSR; 	/* Output Status Register               */
	AT91_REG	 Resv1[1]; 	/*                                      */
	AT91_REG	 PIO_IFER; 	/* Input Filter Enable Register         */
	AT91_REG	 PIO_IFDR; 	/* Input Filter Disable Register        */
	AT91_REG	 PIO_IFSR; 	/* Input Filter Status Register         */
	AT91_REG	 Resv2[1]; 	/*                                      */
	AT91_REG	 PIO_SODR; 	/* Set Output Data Register             */
	AT91_REG	 PIO_CODR; 	/* Clear Output Data Register           */
	AT91_REG	 PIO_ODSR; 	/* Output Data Status Register          */
	AT91_REG	 PIO_PDSR; 	/* Pin Data Status Register             */
	AT91_REG	 PIO_IER; 	/* Interrupt Enable Register            */
	AT91_REG	 PIO_IDR; 	/* Interrupt Disable Register           */
	AT91_REG	 PIO_IMR; 	/* Interrupt Mask Register              */
	AT91_REG	 PIO_ISR; 	/* Interrupt Status Register            */
	AT91_REG	 PIO_MDER; 	/* Multi-driver Enable Register         */
	AT91_REG	 PIO_MDDR; 	/* Multi-driver Disable Register        */
	AT91_REG	 PIO_MDSR; 	/* Multi-driver Status Register         */
	AT91_REG	 Resv3[1]; 	/*                                      */
	AT91_REG	 PIO_PPUDR; 	/* Pull-up Disable Register             */
	AT91_REG	 PIO_PPUER; 	/* Pull-up Enable Register              */
	AT91_REG	 PIO_PPUSR; 	/* Pad Pull-up Status Register          */
	AT91_REG	 Resv4[1]; 	/*                                      */
	AT91_REG	 PIO_ASR; 	/* Select A Register                    */
	AT91_REG	 PIO_BSR; 	/* Select B Register                    */
	AT91_REG	 PIO_ABSR; 	/* AB Select Status Register            */
	AT91_REG	 Resv5[9]; 	/*                                      */
	AT91_REG	 PIO_OWER; 	/* Output Write Enable Register         */
	AT91_REG	 PIO_OWDR; 	/* Output Write Disable Register        */
	AT91_REG	 PIO_OWSR; 	/* Output Write Status Register         */
	AT91_REG	 Resv6[85]; 	/*                                      */
}AT91_PIO_S;                             

#endif

/* PIOA config:
PA0:	clock output 3	/B
PA1:	clock output 0	/B
PA2:	IRQ4			/B
PA3:	IRQ5			/B
PA4:	clock output 1	/B
PA5:	txd3			/B
PA6:	rxd3			/B
PA7:	ETXCK			/A

PA8:	eth enable		/A
PA9:	eth tx0			/A
PA10:	eth tx1			/A
PA11:	eth ECRS		/A
PA12:	eth rx0			/A
PA13:	eth rx1			/A
PA14:	eth ERXER		/A
PA15:	eth EMDC		/A

PA16:	eth EMDIO		/A
PA17:	UART TXD0		/A
PA18:	UART RXD0		/A
PA19:	UART SCK0		/A
PA20:	UART CTS0		/A
PA21:	UART RTS0		/A
PA22:	UART RXD2		/A
PA23:	UART TXD2		/A
PA24:	UART SCK2		/A

PA25:	TWD				/A
PA26:	TWCK			/A
PA27:	TCLK3			/B
PA28:	TCLK4			/B
PA29:	TCLK5			/B

PA30:	DRXD			/A
PA31:	DTXD			/A
  
 */

#define AT91_PIOA_ASR_VALUE	 0xC7FFFF80
#define AT91_PIOA_BSR_VALUE	 0x3800007F
#define AT91_PIOA_OER_VALUE	 (0&(~(AT91_PIOA_ASR_VALUE|AT91_PIOA_BSR_VALUE)))
#define AT91_PIOA_ODR_VALUE	 (~(AT91_PIOA_OER_VALUE)&(~(AT91_PIOA_ASR_VALUE|AT91_PIOA_BSR_VALUE)))



/* PIOB config:
PB0:	RTS3			/B
PB1:	CTS3			/B
PB2:	SCK3			/B
PB3:	MCDA1			/B
PB4:	MCDA2			/B
PB5:	MCDA3			/B
PB6:	TIOA3			/B
PB7:	TIOB3			/B

PB8:	TIOA4			/B
PB9:	TIOB4			/B
PB10:	TIOA5			/B
PB11:	TIOB5			/B
PB12:	ETX2			/B
PB13:	ETX3			/B
PB14:	ETXER			/B
PB15:	ERX2			/B

PB16:	ERX3			/B
PB17:	ERXDV			/B
PB18:	ECOL			/B
PB19:	EXRXCK			/B
PB20:	UART TXD1		/A
PB21:	UART RXD1		/A
PB22:	UART SCK1		/A
PB23:	UART DCD1		/A
PB24:	UART CTS1		/A

PB25:	DSR1			/A
PB26:	RTS1			/A
PB27:	PCK0			/A
PB28:	FIQ				/A
PB29:	IRQ0			/A

PB30:	DUMMY			/0
PB31:	DUMMY			/0
  
 */

#define AT91_PIOB_ASR_VALUE	 0x3FF00000
#define AT91_PIOB_BSR_VALUE	 0x000FFFFF
#define AT91_PIOB_OER_VALUE	 (0&(~(AT91_PIOB_ASR_VALUE|AT91_PIOB_BSR_VALUE)))
#define AT91_PIOB_ODR_VALUE	 (~(AT91_PIOB_OER_VALUE)&(~(AT91_PIOB_ASR_VALUE|AT91_PIOB_BSR_VALUE)))
 
 
/* PIOC config:                           
PC0:	IO			/0        
PC1:	IO			/0        
PC2:	IO			/0        
PC3:	IO			/0        
PC4:	IO			/0        
PC5:	IO			/0        
PC6:	IO			/0        
PC7:	A23			/A        
                                          
PC8:	A24			/A        
PC9:	A24			/A        
PC10:	NCS4		/A        
PC11:	NCS5		/A        
PC12:	NCS6		/A        
PC13:	NCS7		/A        
PC14:	IO			/0        
PC15:	IO			/0        
                                          
PC16:	D16			/A        
PC17:	D17			/A        
PC18:	 			/A        
PC19:	 			/A        
PC20:	   			/A        
PC21:	   			/A        
PC22:	   			/A        
PC23:	   			/A        
PC24:	   			/A        
                                          
PC25:	 			/A        
PC26:	 			/A        
PC27:	 			/A        
PC28:	 			/A
PC29:	 			/A        
                                          
PC30:	 			/A        
PC31:	 			/A        
                                          
 */                                       
#define AT91_PIOC_ASR_VALUE	 0xFFFF3F80
#define AT91_PIOC_BSR_VALUE	 0x00000000
#define AT91_PIOC_OER_VALUE	 (0&(~(AT91_PIOC_ASR_VALUE|AT91_PIOC_BSR_VALUE)))
#define AT91_PIOC_ODR_VALUE	 (~(AT91_PIOC_OER_VALUE)&(~(AT91_PIOC_ASR_VALUE|AT91_PIOC_BSR_VALUE)))

/* PIOD config:                   
PD0:	IO			/0
PD1:	IO			/0
PD2:	IO			/0
PD3:	IO			/0
PD4:	IO			/0
PD5:	IO			/0
PD6:	IO			/0
PD7:	IO			/0
                                  
PD8:	IO			/0
PD9:	IO			/0
PD10:	IO			/0        
PD11:	IO			/0        
PD12:	IO			/0        
PD13:	IO			/0        
PD14:	IO			/0
PD15:	IO			/0
                                  
PD16:	IO			/0
PD17:	IO			/0
PD18:	IO 			/0
PD19:	IO 			/0
PD21:	IO   		/0
PD22:	IO   		/0
PD24:	IO   		/0
                                  
PD25:	IO 			/0
PD26:	IO 			/0
PD27:	IO 			/0
PD28:	IO 			/0
PD29:	IO 			/0
                                  
PD30:	IO 			/0
PD31:	IO 			/0
                                  
 */   

#define AT91_PIOD_ASR_VALUE	 0x00000000
#define AT91_PIOD_BSR_VALUE	 0x00000000
#define AT91_PIOD_OER_VALUE	 (0&(~(AT91_PIOD_ASR_VALUE|AT91_PIOD_BSR_VALUE)))
#define AT91_PIOD_ODR_VALUE	 (~(AT91_PIOD_OER_VALUE)&(~(AT91_PIOD_ASR_VALUE|AT91_PIOD_BSR_VALUE)))



#endif	/*_AT91_PIO_DEFINE_HEADER*/

