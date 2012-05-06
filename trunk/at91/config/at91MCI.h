
#ifndef _AT91_MCI_DEFINE_HEADER
#define _AT91_MCI_DEFINE_HEADER


#define MCI_BASE_ADDR	0xFFFB4000

#ifndef _ASMLANGUAGE

typedef struct _MCI_S
{
	AT91_REG	 MCI_CR; 		/* MCI Control Register				*/
	AT91_REG	 MCI_MR; 		/* MCI Mode Register                */
	AT91_REG	 MCI_DTOR; 		/* MCI Data Timeout Register        */
	AT91_REG	 MCI_SDCR; 		/* MCI SD Card Register             */
	AT91_REG	 MCI_ARGR; 		/* MCI Argument Register            */
	AT91_REG	 MCI_CMDR; 		/* MCI Command Register             */
	AT91_REG	 Reserved0[2]; 	/*                                  */
	AT91_REG	 MCI_RSPR[4]; 	/* MCI Response Register            */
	AT91_REG	 MCI_RDR; 		/* MCI Receive Data Register        */
	AT91_REG	 MCI_TDR; 		/* MCI Transmit Data Register       */
	AT91_REG	 Reserved1[2]; 	/*                                  */
	AT91_REG	 MCI_SR; 		/* MCI Status Register              */
	AT91_REG	 MCI_IER; 		/* MCI Interrupt Enable Register    */
	AT91_REG	 MCI_IDR; 		/* MCI Interrupt Disable Register   */
	AT91_REG	 MCI_IMR; 		/* MCI Interrupt Mask Register      */
	AT91_REG	 Reserved2[44]; 
	PDC_S		 MCI_PDC;
}MCI_S;

#endif



#endif /*_AT91_MCI_DEFINE_HEADER*/

