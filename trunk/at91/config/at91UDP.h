
#ifndef _AT91_UDP_DEFINE_HEADER
#define _AT91_UDP_DEFINE_HEADER



#define UDP_BASE_ADDR	0xFFFB0000

#ifndef _ASMLANGUAGE

typedef struct _UDP_S {
	AT91_REG	 UDP_NUM; 		/* Frame Number Register				*/
	AT91_REG	 UDP_GLBSTATE; 	/* Global State Register                */
	AT91_REG	 UDP_FADDR; 	/* Function Address Register            */
	AT91_REG	 Reserved0[1]; 	/*                                      */
	AT91_REG	 UDP_IER; 		/* Interrupt Enable Register            */
	AT91_REG	 UDP_IDR; 		/* Interrupt Disable Register           */
	AT91_REG	 UDP_IMR; 		/* Interrupt Mask Register              */
	AT91_REG	 UDP_ISR; 		/* Interrupt Status Register            */
	AT91_REG	 UDP_ICR; 		/* Interrupt Clear Register             */
	AT91_REG	 Reserved1[1]; 	/*                                      */
	AT91_REG	 UDP_RSTEP; 	/* Reset Endpoint Register              */
	AT91_REG	 Reserved2[1]; 	/*                                      */
	AT91_REG	 UDP_CSR[8]; 	/* Endpoint Control and Status Register	*/
	AT91_REG	 UDP_FDR[8]; 	/* Endpoint FIFO Data Register          */
}UDP_S;

#endif

#endif /*_AT91_UDP_DEFINE_HEADER*/

