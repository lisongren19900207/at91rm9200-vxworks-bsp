
#ifndef _AT91_PDC_DEFINE_HEADER
#define _AT91_PDC_DEFINE_HEADER



#ifndef _ASMLANGUAGE

typedef struct  _PDC_S
{
	AT91_REG	 PDC_RPR; 			/* Receive Pointer Register                 */
	AT91_REG	 PDC_RCR; 			/* Receive Counter Register                 */
	AT91_REG	 PDC_TPR; 			/* Transmit Pointer Register                */
	AT91_REG	 PDC_TCR; 			/* Transmit Counter Register                */
	AT91_REG	 PDC_RNPR; 			/* Receive Next Pointer Register            */
	AT91_REG	 PDC_RNCR; 			/* Receive Next Counter Register            */
	AT91_REG	 PDC_TNPR; 			/* Transmit Next Pointer Register           */
	AT91_REG	 PDC_TNCR; 			/* Transmit Next Counter Register           */
	AT91_REG	 PDC_PTCR; 			/* PDC Transfer Control Register            */
	AT91_REG	 PDC_PTSR; 			/* PDC Transfer Status Register             */
}PDC_S;
#endif


		/* -------- PDC_PTCR : (PDC Offset: 0x20) PDC Transfer Control Register -------- */
#define PDC_PTCR_RXTEN       ((unsigned int) 0x1 <<  0) /* (PDC) Receiver Transfer Enable	  */
#define PDC_PTCR_RXTDIS      ((unsigned int) 0x1 <<  1) /* (PDC) Receiver Transfer Disable	  */
#define PDC_PTCR_TXTEN       ((unsigned int) 0x1 <<  8) /* (PDC) Transmitter Transfer Enable  */
#define PDC_PTCR_TXTDIS      ((unsigned int) 0x1 <<  9) /* (PDC) Transmitter Transfer Disable */

#endif /*_AT91_PDC_DEFINE_HEADER*/

