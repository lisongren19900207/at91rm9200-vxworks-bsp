
#ifndef _AT91_SSC_DEFINE_HEADER
#define _AT91_SSC_DEFINE_HEADER


#define SSC0_BASE_ADDR	0xFFFD0000
#define SSC1_BASE_ADDR	0xFFFD4000
#define SSC2_BASE_ADDR	0xFFFD8000


#ifndef _ASMLANGUAGE
typedef struct _SSC_S {                                  
	AT91_REG	 SSC_CR; 		/* Control Register					*/              
	AT91_REG	 SSC_CMR; 		/* Clock Mode Register              */
	AT91_REG	 Reserved0[2]; 	/*                                  */
	AT91_REG	 SSC_RCMR; 		/* Receive Clock ModeRegister       */
	AT91_REG	 SSC_RFMR; 		/* Receive Frame Mode Register      */
	AT91_REG	 SSC_TCMR; 		/* Transmit Clock Mode Register     */
	AT91_REG	 SSC_TFMR; 		/* Transmit Frame Mode Register     */
	AT91_REG	 SSC_RHR; 		/* Receive Holding Register         */
	AT91_REG	 SSC_THR; 		/* Transmit Holding Register        */
	AT91_REG	 Reserved1[2]; 	/*                                  */
	AT91_REG	 SSC_RSHR; 		/* Receive Sync Holding Register    */
	AT91_REG	 SSC_TSHR; 		/* Transmit Sync Holding Register   */
	AT91_REG	 SSC_RC0R; 		/* Receive Compare 0 Register       */
	AT91_REG	 SSC_RC1R; 		/* Receive Compare 1 Register       */
	AT91_REG	 SSC_SR; 		/* Status Register                  */
	AT91_REG	 SSC_IER; 		/* Interrupt Enable Register        */
	AT91_REG	 SSC_IDR; 		/* Interrupt Disable Register       */
	AT91_REG	 SSC_IMR; 		/* Interrupt Mask Register          */
	AT91_REG	 Reserved2[44]; /*                                  */
	PDC_S		 SSC_PDC;
}SSC_S;

#endif


#endif /*_AT91_SSC_DEFINE_HEADER*/

