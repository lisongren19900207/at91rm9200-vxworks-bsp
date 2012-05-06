
#ifndef _AT91_TWI_DEFINE_HEADER
#define _AT91_TWI_DEFINE_HEADER


#define TWI_BASE_ADDR	0xFFFB8000

#ifndef _ASMLANGUAGE
typedef struct _TWI_S
{
	AT91_REG	 TWI_CR; 		/* Control Register						*/
	AT91_REG	 TWI_MMR; 		/* Master Mode Register                 */
	AT91_REG	 TWI_SMR; 		/* Slave Mode Register                  */
	AT91_REG	 TWI_IADR; 		/* Internal Address Register            */
	AT91_REG	 TWI_CWGR; 		/* Clock Waveform Generator Register    */
	AT91_REG	 Reserved0[3]; 	/*                                      */
	AT91_REG	 TWI_SR; 		/* Status Register                      */
	AT91_REG	 TWI_IER; 		/* Interrupt Enable Register            */
	AT91_REG	 TWI_IDR; 		/* Interrupt Disable Register           */
	AT91_REG	 TWI_IMR; 		/* Interrupt Mask Register              */
	AT91_REG	 TWI_RHR; 		/* Receive Holding Register             */
	AT91_REG	 TWI_THR; 		/* Transmit Holding Register            */
}TWI_S;
#endif


#endif /*_AT91_TWI_DEFINE_HEADER*/

