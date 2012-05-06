
#ifndef _AT91_SPI_DEFINE_HEADER
#define _AT91_SPI_DEFINE_HEADER


#define SPI_BASE_ADDR	0xFFFE0000

#ifndef _ASMLANGUAGE

typedef struct _SPI_S {
	AT91_REG	 SPI_CR; 			/* Control Register				*/
	AT91_REG	 SPI_MR; 			/* Mode Register                */
	AT91_REG	 SPI_RDR; 			/* Receive Data Register        */
	AT91_REG	 SPI_TDR; 			/* Transmit Data Register       */
	AT91_REG	 SPI_SR; 			/* Status Register              */
	AT91_REG	 SPI_IER; 			/* Interrupt Enable Register    */
	AT91_REG	 SPI_IDR; 			/* Interrupt Disable Register   */
	AT91_REG	 SPI_IMR; 			/* Interrupt Mask Register      */
	AT91_REG	 Reserved0[4]; 		/*                              */
	AT91_REG	 SPI_CSR[4]; 		/* Chip Select Register         */
	AT91_REG	 Reserved1[48]; 	/*                              */
	PDC_S		 SPI_PDC;
}SPI_S;

#endif


#endif /*_AT91_SPI_DEFINE_HEADER*/

