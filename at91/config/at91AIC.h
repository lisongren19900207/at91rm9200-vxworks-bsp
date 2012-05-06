
#ifndef _AT91_AIC_DEFINE_HEADER
#define _AT91_AIC_DEFINE_HEADER


#define	AIC_BASE_ADDR	0xFFFFF000
#define AT91C_BASE_CKGR           (0xFFFFFC20) /* (CKGR) Base Address*/
#define	AIC_IDCR_OFFSET	0x124	/* Interrupt Disable Command Register */
#define AIC_ICCR_OFFSET	0x128	/* Interrupt Clear Command Register   */
#define	AIC_INT_NUM_LEVELS	32	
#define	AT91_SRAM_START_ADDR	0x200000
#define AT91C_CKGR_MOSCEN         (0x1 <<  0)	/* */
#define AT91C_CKGR_OSCOUNT        (0xFF <<  8) /* */	
	
#ifndef	_ASMLANGUAGE
	typedef	struct _AIC_S
	{
		AT91_REG AIC_SMR[AIC_INT_NUM_LEVELS];	/*Source Mode Register 0*/  
		AT91_REG AIC_SVR[AIC_INT_NUM_LEVELS];	/*Source Vector	Register 0*/
		AT91_REG AIC_IVR;		/*Interrupt Vector Register*/	  
		AT91_REG AIC_FVR;		/*Fast Interrupt Vector	Register*/
		AT91_REG AIC_ISR;		/*Interrupt Status Register*/	  
		AT91_REG AIC_IPR;		/*Interrupt Pending Register*/	  
		AT91_REG AIC_IMR;		/*Interrupt Mask Register*/	  
		AT91_REG AIC_CISR;		/*Core Interrupt Status	Register*/
		AT91_REG resv0;
		AT91_REG resv1;
		AT91_REG AIC_IECR;		/*Interrupt Enable Command Register*/ 
		AT91_REG AIC_IDCR;		/*Interrupt Disable Command Register*/
		AT91_REG AIC_ICCR;		/*Interrupt Clear Command Register*/  
		AT91_REG AIC_ISCR;		/*Interrupt Set	Command	Register*/    
		AT91_REG AIC_EOICR;		/*End of Interrupt Command Register*/ 
		AT91_REG AIC_SPU;		/*Spurious Interrupt Vector Register*/
		AT91_REG AIC_DCR;		/*Debug	Control	Register*/	      
		AT91_REG resv2;
		AT91_REG AIC_FFER;		/*Fast Forcing Enable Register*/			
		AT91_REG AIC_FFDR;		/*Fast Forcing Disable Register	AIC_FFDR Write-only */	
		AT91_REG AIC_FFSR;		/*Fast Forcing Status Register AIC_FFSR	Read-only 0x0*/	
	}AIC_S;	
	
#endif
	
	
#define	AIC_SMR_EDGETRIG	(1<<5)
#define AIC_SMR_PRIO_BIT	0

	
#define	INT_LVL_FIQ					PHER_AIC 		/*Advanced Interrupt Controller	FIQ*/
#define	INT_LVL_SYSIRQ				PHER_SYSIRQ 	
#define	INT_LVL_PIOA				PHER_PIOA 		/*Parallel I/O Controller A*/
#define	INT_LVL_PIOB				PHER_PIOB 		/*Parallel I/O Controller B*/
#define	INT_LVL_PIOC				PHER_PIOC 		/*Parallel I/O Controller C*/
#define	INT_LVL_PIOD				PHER_PIOD 		/*Parallel I/O Controller D*/
#define	INT_LVL_UART0				PHER_US0 		/* UART	0 */
#define	INT_LVL_UART1				PHER_US1 		/* UART	1 */
#define	INT_LVL_UART2				PHER_US2 		/* UART	2 */
#define	INT_LVL_UART3				PHER_US3 		/* UART	3 */
#define	INT_LVL_MULT_CARD			PHER_MCI 		/*Multimedia Card Interface*/
#define	INT_LVL_USB_DEV_PORT		PHER_UDP 		/*UDP USB Device Port*/	
#define	INT_LVL_TWO_WIRE_IF			PHER_TWI 		/*Two-wire Interface*/
#define	INT_LVL_SPI					PHER_SPI 		/*Serial Peripheral Interface*/	
#define	INT_LVL_SSC0				PHER_SSC0 		/*Synchronous Serial Controller	0*/
#define	INT_LVL_SSC1				PHER_SSC1 		/*Synchronous Serial Controller	1*/
#define	INT_LVL_SSC2				PHER_SSC2 		/*Synchronous Serial Controller	2*/
#define	INT_LVL_TC0					PHER_TC0 		/*Timer/Counter	0*/
#define	INT_LVL_TC1					PHER_TC1 		/*Timer/Counter	1*/
#define	INT_LVL_TC2					PHER_TC2 		/*Timer/Counter	2*/
#define	INT_LVL_TC3					PHER_TC3 		/*Timer/Counter	3*/
#define	INT_LVL_TC4					PHER_TC4 		/*Timer/Counter	4*/
#define	INT_LVL_TC5					PHER_TC5 		/*Timer/Counter	5*/
#define	INT_LVL_USB_HOST_PORT		PHER_UHP 		/*USB Host Port*/
#define	INT_LVL_EMAC				PHER_EMAC  		/*Ethernet MAC*/
#define	INT_LVL_EXT_IRQ0			PHER_AICIRQ0		
#define	INT_LVL_EXT_IRQ1			PHER_AICIRQ1			
#define	INT_LVL_EXT_IRQ2			PHER_AICIRQ2			
#define	INT_LVL_EXT_IRQ3			PHER_AICIRQ3			
#define	INT_LVL_EXT_IRQ4			PHER_AICIRQ4			
#define	INT_LVL_EXT_IRQ5			PHER_AICIRQ5			
#define	INT_LVL_EXT_IRQ6			PHER_AICIRQ6			
	
	
#define	INT_VEC_FIQ				IVEC_TO_INUM(INT_LVL_FIQ)			
#define	INT_VEC_SYSIRQ			IVEC_TO_INUM(INT_LVL_SYSIRQ)		
	
#define	INT_VEC_PIOA			IVEC_TO_INUM(INT_LVL_PIOA)		
#define	INT_VEC_PIOB			IVEC_TO_INUM(INT_LVL_PIOB)		
#define	INT_VEC_PIOC			IVEC_TO_INUM(INT_LVL_PIOC)		
#define	INT_VEC_PIOD			IVEC_TO_INUM(INT_LVL_PIOD)		
	
#define	INT_VEC_UART0			IVEC_TO_INUM(INT_LVL_UART0)		
#define	INT_VEC_UART1			IVEC_TO_INUM(INT_LVL_UART1)		
#define	INT_VEC_UART2			IVEC_TO_INUM(INT_LVL_UART2)		
#define	INT_VEC_UART3			IVEC_TO_INUM(INT_LVL_UART3)		
	
#define	INT_VEC_MULT_CARD		IVEC_TO_INUM(INT_LVL_MULT_CARD)	      
#define	INT_VEC_USB_DEV_PORT	IVEC_TO_INUM(INT_LVL_USB_DEV_PORT)	      
#define	INT_VEC_TWO_WIRE_IF		IVEC_TO_INUM(INT_LVL_TWO_WIRE_IF)	      
#define	INT_VEC_SPI				IVEC_TO_INUM(INT_LVL_SPI)		      
#define	INT_VEC_SSC0			IVEC_TO_INUM(INT_LVL_SSC0)		      
#define	INT_VEC_SSC1			IVEC_TO_INUM(INT_LVL_SSC1)		      
#define	INT_VEC_SSC2			IVEC_TO_INUM(INT_LVL_SSC2)		      
	
#define	INT_VEC_TC0				IVEC_TO_INUM(INT_LVL_TC0)		      
#define	INT_VEC_TC1				IVEC_TO_INUM(INT_LVL_TC1)		      
#define	INT_VEC_TC2				IVEC_TO_INUM(INT_LVL_TC2)		      
#define	INT_VEC_TC3				IVEC_TO_INUM(INT_LVL_TC3)		      
#define	INT_VEC_TC4				IVEC_TO_INUM(INT_LVL_TC4)		      
#define	INT_VEC_TC5				IVEC_TO_INUM(INT_LVL_TC5)		      
#define	INT_VEC_USB_HOST_PORT	IVEC_TO_INUM(INT_LVL_USB_HOST_PORT)	      
#define	INT_VEC_EMAC			IVEC_TO_INUM(INT_LVL_EMAC)		      
#define	INT_VEC_EXT_IRQ0		IVEC_TO_INUM(INT_LVL_EXT_IRQ0)	      
#define	INT_VEC_EXT_IRQ1		IVEC_TO_INUM(INT_LVL_EXT_IRQ1)	      
#define	INT_VEC_EXT_IRQ2		IVEC_TO_INUM(INT_LVL_EXT_IRQ2)	      
#define	INT_VEC_EXT_IRQ3		IVEC_TO_INUM(INT_LVL_EXT_IRQ3)	      
#define	INT_VEC_EXT_IRQ4		IVEC_TO_INUM(INT_LVL_EXT_IRQ4)	      
#define	INT_VEC_EXT_IRQ5		IVEC_TO_INUM(INT_LVL_EXT_IRQ5)	      
#define	INT_VEC_EXT_IRQ6		IVEC_TO_INUM(INT_LVL_EXT_IRQ6)	

#endif /*_AT91_AIC_DEFINE_HEADER*/

