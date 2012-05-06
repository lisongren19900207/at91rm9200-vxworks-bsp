/* at91rm9200.h - Atmel AT91 register definition */

/*
modification history
--------------------
01a,09sep04,pdr	 written.
*/

#ifndef __INCat91rm9200h
#define __INCat91rm9200h

#include "bits.h"

/* *****************************************************************************      
 *               BASE ADDRESS DEFINITIONS FOR AT91RM9200                              
 * *****************************************************************************/     
#define AT91C_BASE_SYS       (0xFFFFF000) /* (SYS) Base Address        */             
#define AT91C_BASE_MC        (0xFFFFFF00) /* (MC) Base Address         */             
#define AT91C_BASE_RTC       (0xFFFFFE00) /* (RTC) Base Address        */             
#define AT91C_BASE_ST        (0xFFFFFD00) /* (ST) Base Address         */             
#define AT91C_BASE_PMC       (0xFFFFFC00) /* (PMC) Base Address        */
#define AT91C_BASE_CKGR      (0xFFFFFC20) /* (CKGR) Base Address       */
#define AT91C_BASE_PIOD      (0xFFFFFA00) /* (PIOD) Base Address       */
#define AT91C_BASE_PIOC      (0xFFFFF800) /* (PIOC) Base Address       */
#define AT91C_BASE_PIOB      (0xFFFFF600) /* (PIOB) Base Address       */ 
#define AT91C_BASE_PIOA      (0xFFFFF400) /* (PIOA) Base Address       */ 
#define AT91C_BASE_DBGU      (0xFFFFF200) /* (DBGU) Base Address       */ 
#define AT91C_BASE_PDC_DBGU  (0xFFFFF300) /* (PDC_DBGU) Base Address   */ 
#define AT91C_BASE_AIC       (0xFFFFF000) /* (AIC) Base Address        */ 
#define AT91C_BASE_PDC_SPI   (0xFFFE0100) /* (PDC_SPI) Base Address    */ 
#define AT91C_BASE_SPI       (0xFFFE0000) /* (SPI) Base Address        */ 
#define AT91C_BASE_PDC_SSC2  (0xFFFD8100) /* (PDC_SSC2) Base Address   */ 
#define AT91C_BASE_SSC2      (0xFFFD8000) /* (SSC2) Base Address       */ 
#define AT91C_BASE_PDC_SSC1  (0xFFFD4100) /* (PDC_SSC1) Base Address   */ 
#define AT91C_BASE_SSC1      (0xFFFD4000) /* (SSC1) Base Address       */ 
#define AT91C_BASE_PDC_SSC0  (0xFFFD0100) /* (PDC_SSC0) Base Address   */ 
#define AT91C_BASE_SSC0      (0xFFFD0000) /* (SSC0) Base Address       */ 
#define AT91C_BASE_PDC_US3   (0xFFFCC100) /* (PDC_US3) Base Address    */ 
#define AT91C_BASE_US3       (0xFFFCC000) /* (US3) Base Address        */ 
#define AT91C_BASE_PDC_US2   (0xFFFC8100) /* (PDC_US2) Base Address    */ 
#define AT91C_BASE_US2       (0xFFFC8000) /* (US2) Base Address        */ 
#define AT91C_BASE_PDC_US1   (0xFFFC4100) /* (PDC_US1) Base Address    */ 
#define AT91C_BASE_US1       (0xFFFC4000) /* (US1) Base Address        */ 
#define AT91C_BASE_PDC_US0   (0xFFFC0100) /* (PDC_US0) Base Address    */ 
#define AT91C_BASE_US0       (0xFFFC0000) /* (US0) Base Address        */ 
#define AT91C_BASE_PDC_TWI   (0xFFFB8100) /* (PDC_TWI) Base Address    */ 
#define AT91C_BASE_TWI       (0xFFFB8000) /* (TWI) Base Address        */ 
#define AT91C_BASE_MCI       (0xFFFB4000) /* (MCI) Base Address        */ 
#define AT91C_BASE_UDP       (0xFFFB0000) /* (UDP) Base Address        */ 
#define AT91C_BASE_TC5       (0xFFFA4080) /* (TC5) Base Address        */ 
#define AT91C_BASE_TC4       (0xFFFA4040) /* (TC4) Base Address        */ 
#define AT91C_BASE_TC3       (0xFFFA4000) /* (TC3) Base Address        */ 
#define AT91C_BASE_TCB1      (0xFFFA4080) /* (TCB1) Base Address       */ 
#define AT91C_BASE_TC2       (0xFFFA0080) /* (TC2) Base Address        */ 
#define AT91C_BASE_TC1       (0xFFFA0040) /* (TC1) Base Address        */ 
#define AT91C_BASE_TC0       (0xFFFA0000) /* (TC0) Base Address        */ 
#define AT91C_BASE_TCB0      (0xFFFA0000) /* (TCB0) Base Address       */ 
#define AT91C_BASE_UHP       (0x00300000) /* (UHP) Base Address        */ 
#define AT91C_BASE_EMAC      (0xFFFBC000) /* (EMAC) Base Address       */ 
#define AT91C_BASE_EBI       (0xFFFFFF60) /* (EBI) Base Address        */ 
#define AT91C_BASE_SMC       (0xFFFFFF70) /* (SMC) Base Address       */ 
#define AT91C_BASE_SDRAMC    (0xFFFFFF90) /* (SDRAMC) Base Address       */ 
#define AT91C_BASE_BFC       (0xFFFFFFC0) /* (BFC) Base Address        */ 

/******************************************************************************
 *               MEMORY MAPPING DEFINITIONS FOR AT91RM9200
 ******************************************************************************/
#define AT91C_ISRAM	        (0x00200000)        /* Internal SRAM base address */
#define AT91C_ISRAM_SIZE	(0x00004000)        /* Internal SRAM size in byte (16 Kbyte) */
#define AT91C_IROM 	        (0x00100000)        /* Internal ROM base address */
#define AT91C_IROM_SIZE	    (0x00020000)        /* Internal ROM size in byte (128 Kbyte) */

/*****************************************************************************
 * Advanced Interrupt Controller (AIC)
 *****************************************************************************/

/* Source Mode Register - 32 of them */
#define AIC_SMR_BASE	0xFFFFF000
#define AIC_SMR_REG(x)	*(volatile UINT32 *)(AIC_SMR_BASE + ((x) & 0x1f))

/* Source Vector Register - 32 of them */
#define AIC_SVR_BASE	0xFFFFF080
#define AIC_SVR_REG(x)	*(volatile UINT32 *)(AIC_SVR_BASE + ((x) & 0x1f))

/* Register Offsets */
#define AIC_IVR 	0x100	/* IRQ Vector Register                */
#define AIC_FVR 	0x104	/* FIQ Vector Register                */
#define AIC_ISR 	0x108	/* Interrupt Status Register          */
#define AIC_IPR 	0x10C	/* Interrupt Pending Register         */
#define AIC_IMR 	0x110	/* Interrupt Mask Register            */
#define AIC_CISR 	0x114	/* Core Interrupt Status Register     */
#define AIC_IECR 	0x120	/* Interrupt Enable Command Register  */
#define AIC_IDCR 	0x124	/* Interrupt Disable Command Register */
#define AIC_ICCR 	0x128	/* Interrupt Clear Command Register   */
#define AIC_ISCR 	0x12C	/* Interrupt Set Command Register     */
#define AIC_EOICR 	0x130	/* End of Interrupt Command Register  */
#define AIC_SPU 	0x134	/* Spurious Vector Register           */
#define AIC_DCR 	0x138	/* Debug Control Register (Protect)   */
#define AIC_FFER 	0x140	/* Fast Forcing Enable Register       */
#define AIC_FFDR 	0x144	/* Fast Forcing Disable Register      */
#define AIC_FFSR 	0x148	/* Fast Forcing Status Register       */

/* ========== Register definition for AIC peripheral ========== */
#define AT91C_AIC_ICCR  (AT91C_BASE_AIC + AIC_ICCR)  /* (AIC) Interrupt Clear Command Register      */ 
#define AT91C_AIC_IECR  (AT91C_BASE_AIC + AIC_IECR)  /* (AIC) Interrupt Enable Command Register     */ 
#define AT91C_AIC_SMR   (AIC_SMR_BASE)               /* (AIC) Source Mode Register                  */ 
#define AT91C_AIC_ISCR  (AT91C_BASE_AIC + AIC_ISCR)  /* (AIC) Interrupt Set Command Register        */ 
#define AT91C_AIC_EOICR (AT91C_BASE_AIC + AIC_EOICR) /* (AIC) End of Interrupt Command Register     */ 
#define AT91C_AIC_DCR   (AT91C_BASE_AIC + AIC_DCR)   /* (AIC) Debug Control Register (Protect)      */ 
#define AT91C_AIC_FFER  (AT91C_BASE_AIC + AIC_FFER)  /* (AIC) Fast Forcing Enable Register          */ 
#define AT91C_AIC_SVR   (AIC_SVR_BASE)               /* (AIC) Source Vector Register                */ 
#define AT91C_AIC_SPU   (AT91C_BASE_AIC + AIC_SPU)   /* (AIC) Spurious Vector Register              */ 
#define AT91C_AIC_FFDR  (AT91C_BASE_AIC + AIC_FFDR)  /* (AIC) Fast Forcing Disable Register         */ 
#define AT91C_AIC_FVR   (AT91C_BASE_AIC + AIC_FVR)   /* (AIC) FIQ Vector Register                   */ 
#define AT91C_AIC_FFSR  (AT91C_BASE_AIC + AIC_FFSR)  /* (AIC) Fast Forcing Status Register          */ 
#define AT91C_AIC_IMR   (AT91C_BASE_AIC + AIC_IMR)   /* (AIC) Interrupt Mask Register               */ 
#define AT91C_AIC_ISR   (AT91C_BASE_AIC + AIC_ISR)   /* (AIC) Interrupt Status Register             */ 
#define AT91C_AIC_IVR   (AT91C_BASE_AIC + AIC_IVR)   /* (AIC) IRQ Vector Register                   */ 
#define AT91C_AIC_IDCR  (AT91C_BASE_AIC + AIC_IDCR)  /* (AIC) Interrupt Disable Command Register    */ 
#define AT91C_AIC_CISR  (AT91C_BASE_AIC + AIC_CISR)  /* (AIC) Core Interrupt Status Register        */
#define AT91C_AIC_IPR   (AT91C_BASE_AIC + AIC_IPR)   /* (AIC) Interrupt Pending Register            */

/* -------- AIC_SMR : (AIC Offset: 0x0) Control Register -------- */ 
#define AT91C_AIC_PRIOR         (0x7) /* (AIC) Priority Level */
#define AT91C_AIC_PRIOR_LOWEST  (0x0) /* (AIC) Lowest priority level */
#define AT91C_AIC_PRIOR_HIGHEST (0x7) /* (AIC) Highest priority level */

#define AT91C_AIC_SRCTYPE                      (0x3) /* (AIC) Interrupt Source Type */
#define AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE  (0x0) /* (AIC) Internal Sources Code Label Level Sensitive */
#define AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED   (0x1) /* (AIC) Internal Sources Code Label Edge triggered */
#define AT91C_AIC_SRCTYPE_EXT_HIGH_LEVEL       (0x2) /* (AIC) External Sources Code Label High-level Sensitive */
#define AT91C_AIC_SRCTYPE_EXT_POSITIVE_EDGE    (0x3) /* (AIC) External Sources Code Label Positive Edge triggered */
                                                 
/* -------- AIC_CISR : (AIC Offset: 0x114) AIC Core Interrupt Status Register -------- */ 
#define AT91C_AIC_NFIQ        ((unsigned int) 0x1 <<  0) /* (AIC) NFIQ Status */
#define AT91C_AIC_NIRQ        ((unsigned int) 0x1 <<  1) /* (AIC) NIRQ Status */

/* -------- AIC_DCR : (AIC Offset: 0x138) AIC Debug Control Register (Protect) -------- */ 
#define AT91C_AIC_DCR_PROT    ((unsigned int) 0x1 <<  0) /* (AIC) Protection Mode */
#define AT91C_AIC_DCR_GMSK    ((unsigned int) 0x1 <<  1) /* (AIC) General Mask */

/* *****************************************************************************
 * Debug Unit (DBGU)
 * *****************************************************************************/
#define DBGU_BASE	0xFFFFF200
#define DBGU_REG(x)	*(volatile UINT32 *)(DBGU_BASE + (x))

/* Register Offsets */
#define DBGU_CR 	0x00	/* Control Register             */
#define DBGU_MR 	0x04	/* Mode Register                */
#define DBGU_IER 	0x08	/* Interrupt Enable Register    */
#define DBGU_IDR 	0x0C	/* Interrupt Disable Register   */
#define DBGU_IMR 	0x10	/* Interrupt Mask Register      */
#define DBGU_SR 	0x14	/* Channel Status Register      */
#define DBGU_RHR 	0x18	/* Receiver Holding Register    */
#define DBGU_THR 	0x1C	/* Transmitter Holding Register */
#define DBGU_BRGR 	0x20	/* Baud Rate Generator Register */
#define DBGU_C1R 	0x40	/* Chip ID1 Register            */
#define DBGU_C2R 	0x44	/* Chip ID2 Register            */
#define DBGU_FNTR 	0x48	/* Force NTRST Register         */
                                                                        
/* *****************************************************************************
 * Peripheral Data Control (DMA)
 * Note that each of the following peripherals has it's own
 * set of these registers starting at offset 0x100 from it's
 * base address: DBGU, SPI, USART and SSC
 * To access the DMA for a peripheral, use the macro for that 
 * peripheral but with these register offsets
 * *****************************************************************************/
/* Register Offsets */
#define PDC_RPR 	0x100	/* Receive Pointer Register          */ 
#define PDC_RCR 	0x104	/* Receive Counter Register          */ 
#define PDC_TPR 	0x108	/* Transmit Pointer Register         */ 
#define PDC_TCR 	0x10c	/* Transmit Counter Register         */ 
#define PDC_RNPR 	0x110	/* Receive Next Pointer Register     */ 
#define PDC_RNCR 	0x114	/* Receive Next Counter Register     */ 
#define PDC_TNPR 	0x118	/* Transmit Next Pointer Register    */ 
#define PDC_TNCR 	0x11c	/* Transmit Next Counter Register    */
#define PDC_PTCR 	0x120	/* PDC Transfer Control Register     */
#define PDC_PTSR 	0x124	/* PDC Transfer Status Register      */

/* -------- PDC_PTCR : (PDC Offset: 0x20) PDC Transfer Control Register -------- */
#define PDC_RXTEN       ((unsigned int) 0x1 <<  0) /* (PDC) Receiver Transfer Enable     */ 
#define PDC_RXTDIS      ((unsigned int) 0x1 <<  1) /* (PDC) Receiver Transfer Disable    */ 
#define PDC_TXTEN       ((unsigned int) 0x1 <<  8) /* (PDC) Transmitter Transfer Enable  */ 
#define PDC_TXTDIS      ((unsigned int) 0x1 <<  9) /* (PDC) Transmitter Transfer Disable */ 


/* *****************************************************************************
 * System Peripheral Interface (SPI)
 * *****************************************************************************/
#define SPI_BASE	AT91C_BASE_SPI
#define SPI_REG(x)	*(volatile UINT32 *)(SPI_BASE + (x))

/* Register Offsets */
#define SPI_CR    0x00 /* (SPI) Control Register           */
#define SPI_MR    0x04 /* (SPI) Mode Register              */
#define SPI_RDR   0x08 /* (SPI) Receive Data Register      */
#define SPI_TDR   0x0C /* (SPI) Transmit Data Register     */
#define SPI_SR    0x10 /* (SPI) Status Register            */
#define SPI_IER   0x14 /* (SPI) Interrupt Enable Register  */
#define SPI_IDR   0x18 /* (SPI) Interrupt Disable Register */
#define SPI_IMR   0x1C /* (SPI) Interrupt Mask Register    */
#define SPI_CSR   0x30 /* (SPI) Chip Select Register       */

/* *****************************************************************************
 * Two Wire Interface (TWI)
 * *****************************************************************************/
#define TWI_BASE	AT91C_BASE_TWI
#define TWI_REG(x)	*(volatile UINT32 *)(TWI_BASE + (x))

/* Register Offsets */
#define TWI_RHR   0x30 /* (TWI) Receive Holding Register            */ 
#define TWI_IDR   0x28 /* (TWI) Interrupt Disable Register          */ 
#define TWI_SR    0x20 /* (TWI) Status Register                     */ 
#define TWI_CWGR  0x10 /* (TWI) Clock Waveform Generator Register   */ 
#define TWI_SMR   0x08 /* (TWI) Slave Mode Register                 */ 
#define TWI_CR    0x00 /* (TWI) Control Register                    */ 
#define TWI_THR   0x34 /* (TWI) Transmit Holding Register           */ 
#define TWI_IMR   0x2C /* (TWI) Interrupt Mask Register             */ 
#define TWI_IER   0x24 /* (TWI) Interrupt Enable Register           */ 
#define TWI_IADR  0x0C /* (TWI) Internal Address Register            */
#define TWI_MMR   0x04 /* (TWI) Master Mode Register                */

/* *****************************************************************************
 * Universal Synchronous Asynchronous Receiver Transceiver (USART)
 * *****************************************************************************/
#define USART0_BASE	    AT91C_BASE_US0
#define USART0_REG(x)	*(volatile UINT32 *)(USART0_BASE + (x))
#define USART1_BASE	    AT91C_BASE_US1
#define USART1_REG(x)	*(volatile UINT32 *)(USART1_BASE + (x))
#define USART2_BASE	    AT91C_BASE_US2
#define USART2_REG(x)	*(volatile UINT32 *)(USART2_BASE + (x))
#define USART3_BASE	    AT91C_BASE_US3
#define USART3_REG(x)	*(volatile UINT32 *)(USART3_BASE + (x))

/* Register Offsets */

#define US_CR    0x00 /* (USART) Receive Holding Register        */ 
#define US_MR    0x04 /* (USART) Mode Register                   */
#define US_IER   0x08 /* (USART) Interrupt Enable Register       */
#define US_IDR   0x0C /* (USART) Interrupt Disable Register      */
#define US_IMR   0x10 /* (USART) Interrupt Mask Register         */
#define US_CSR   0x14 /* (USART) Channel Status Register         */
#define US_RHR   0x18 /* (USART) Receiver Holding Register       */
#define US_THR   0x1C /* (USART) Transmitter Holding Register    */
#define US_BRGR  0x20 /* (USART) Baud Rate Generator Register    */ 
#define US_RTOR  0x24 /* (USART) Receiver Time-out Register      */
#define US_TTGR  0x28 /* (USART) Transmitter Time-guard Register */
#define US_FDI   0x40 /* (USART) FI_DI_Ratio Register            */
#define US_NER   0x44 /* (USART) Nb Errors Register              */
#define US_IF    0x4C /* (USART) IRDA_FILTER Register            */

/* *****************************************************************************
 * Serial Synchronous Controller (SSC)
 * *****************************************************************************/
#define SSC0_BASE	    AT91C_BASE_SSC0
#define SSC0_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_SSC0 + (x))
#define SSC1_BASE	    AT91C_BASE_SSC1
#define SSC1_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_SSC1 + (x))
#define SSC2_BASE	    AT91C_BASE_SSC2
#define SSC2_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_SSC2 + (x))

/* Register Offsets */
#define SSC_CR      0x00    /* (SSC) Control Register                */     
#define SSC_CMR     0x04    /* (SSC) Clock Mode Register             */     
#define SSC_RCMR    0x10    /* (SSC) Receive Clock ModeRegister      */                               
#define SSC_RFMR    0x14    /* (SSC) Receive Frame Mode Register     */     
#define SSC_TCMR    0x18    /* (SSC) Transmit Clock Mode Register    */     
#define SSC_TFMR    0x1C    /* (SSC) Transmit Frame Mode Register    */     
#define SSC_RHR     0x20    /* (SSC) Receive Holding Register        */     
#define SSC_THR     0x24    /* (SSC) Transmit Holding Register       */     
#define SSC_RSHR    0x30    /* (SSC) Receive Sync Holding Register   */       
#define SSC_TSHR    0x34    /* (SSC) Transmit Sync Holding Register  */                               
#define SSC_SR      0x40    /* (SSC) Status Register                 */    
#define SSC_IER     0x44    /* (SSC) Interrupt Enable Register       */    
#define SSC_IDR     0x48    /* (SSC) Interrupt Disable Register      */     
#define SSC_IMR     0x4C    /* (SSC) Interrupt Mask Register         */     
                                                                      
/* *****************************************************************************
 * Timer Counter (TC)
 * *****************************************************************************/
#define TC0_BASE	    AT91C_BASE_TC0
#define TC0_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_TC0 + (x))
#define TC1_BASE	    AT91C_BASE_TC1
#define TC1_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_TC1 + (x))
#define TC2_BASE	    AT91C_BASE_TC2
#define TC2_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_TC2 + (x))
#define TC3_BASE	    AT91C_BASE_TC3
#define TC3_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_TC3 + (x))
#define TC4_BASE	    AT91C_BASE_TC4
#define TC4_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_TC4 + (x))
#define TC5_BASE	    AT91C_BASE_TC5
#define TC5_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_TC5 + (x))

/* Register Offsets */
#define TC_BCR 0xC0     /* (TC) Block Control Register       */        
#define TC_BMR 0xC4     /* (TC) Block Mode Register          */ 
                                        
#define TC_CCR  0x00    /* (TC) Channel Control Register     */  
#define TC_CMR  0x04    /* (TC) Channel Mode Register        */  
#define TC_CV   0x10    /* (TC) Counter Value                */    
#define TC_RA   0x14    /* (TC) Register A                   */    
#define TC_RB   0x18    /* (TC) Register B                   */    
#define TC_RC   0x1C    /* (TC) Register C                   */    
#define TC_SR   0x20    /* (TC) Status Register              */    
#define TC_IER  0x24    /* (TC) Interrupt Enable Register    */ 
#define TC_IDR  0x28    /* (TC) Interrupt Disable Register   */ 
#define TC_IMR  0x2C    /* (TC) Interrupt Mask Register      */ 
                                                                
                                                             
                                                             
/* *****************************************************************************
 * MultiMedia Card Interface (MCI)
 * *****************************************************************************/
#define MCI_BASE	    AT91C_BASE_MCI
#define MCI_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_MCI + (x))

/* Register Offsets */
#define MCI_CR    0x00 /* (MCI) MCI Control Register             */ 
#define MCI_MR    0x04 /* (MCI) MCI Mode Register                */
#define MCI_DTOR  0x08 /* (MCI) MCI Data Timeout Register        */ 
#define MCI_SDCR  0x0C /* (MCI) MCI SD Card Register             */
#define MCI_ARGR  0x10 /* (MCI) MCI Argument Register            */ 
#define MCI_CMDR  0x14 /* (MCI) MCI Command Register             */
#define MCI_RSPR  0x20 /* (MCI) MCI Response Register            */ 
#define MCI_RDR   0x30 /* (MCI) MCI Receive Data Register        */ 
#define MCI_TDR   0x34 /* (MCI) MCI Transmit Data Register       */ 
#define MCI_SR    0x40 /* (MCI) MCI Status Register              */ 
#define MCI_IER   0x44 /* (MCI) MCI Interrupt Enable Register    */ 
#define MCI_IDR   0x48 /* (MCI) MCI Interrupt Disable Register   */ 
#define MCI_IMR   0x4C /* (MCI) MCI Interrupt Mask Register      */ 


/* *****************************************************************************
 * USB Device Port (UDP)
 * *****************************************************************************/
#define UDP_BASE	    AT91C_BASE_UDP
#define UDP_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_UDP + (x))

/* Register Offsets */
#define UDP_IDR   0x14 /* (UDP) Interrupt Disable Register               */ 
#define UDP_CSR0  0x30 /* (UDP) Endpoint 0 Control and Status Register   */ 
#define UDP_CSR1  0x34 /* (UDP) Endpoint 1 Control and Status Register   */ 
#define UDP_CSR2  0x38 /* (UDP) Endpoint 2 Control and Status Register   */ 
#define UDP_CSR3  0x3C /* (UDP) Endpoint 3 Control and Status Register   */ 
#define UDP_CSR4  0x40 /* (UDP) Endpoint 4 Control and Status Register   */ 
#define UDP_CSR5  0x44 /* (UDP) Endpoint 5 Control and Status Register   */ 
#define UDP_CSR6  0x48 /* (UDP) Endpoint 6 Control and Status Register   */ 
#define UDP_CSR7  0x4C /* (UDP) Endpoint 7 Control and Status Register   */ 

/* *****************************************************************************
 * USB Host Port (UHP)
 * *****************************************************************************/
#define UHP_BASE	    AT91C_BASE_UHP
#define UHP_REG(x)	    *(volatile UINT32 *)(AT91C_BASE_UHP + (x))

/* *****************************************************************************
 * Parallel I/O Unit (PIO)
 * There are four PIO blocks - A, B, C and D.  They all have the
 * same register set, but different base addresses
 * *****************************************************************************/
/* Port A */
#define PIOA_BASE		0xFFFFF400
#define PIOA_REG(x)	    *(volatile UINT32 *)(PIOA_BASE + (x))

/* Port B */
#define PIOB_BASE		0xFFFFF600
#define PIOB_REG(x)	    *(volatile UINT32 *)(PIOB_BASE + (x))

/* Port C */
#define PIOC_BASE		0xFFFFF800
#define PIOC_REG(x)	    *(volatile UINT32 *)(PIOC_BASE + (x))

/* Port D */
#define PIOD_BASE		0xFFFFFA00
#define PIOD_REG(x)	    *(volatile UINT32 *)(PIOD_BASE + (x))

/* Registers */
#define PIO_PER 	0x00	/* PIO Enable Register              */ 
#define PIO_PDR 	0x04	/* PIO Disable Register             */ 
#define PIO_PSR 	0x08	/* PIO Status Register              */ 
#define PIO_OER 	0x10	/* Output Enable Register           */ 
#define PIO_ODR 	0x14	/* Output Disable Registerr         */ 
#define PIO_OSR 	0x18	/* Output Status Register           */ 
#define PIO_IFER 	0x20	/* Input Filter Enable Register     */ 
#define PIO_IFDR 	0x24	/* Input Filter Disable Register    */ 
#define PIO_IFSR 	0x28	/* Input Filter Status Register     */ 
#define PIO_SODR 	0x30	/* Set Output Data Register         */ 
#define PIO_CODR 	0x34	/* Clear Output Data Register       */ 
#define PIO_ODSR 	0x38	/* Output Data Status Register      */ 
#define PIO_PDSR 	0x3c	/* Pin Data Status Register         */ 
#define PIO_IER 	0x40	/* Interrupt Enable Register        */ 
#define PIO_IDR 	0x44	/* Interrupt Disable Register       */ 
#define PIO_IMR 	0x48	/* Interrupt Mask Register          */ 
#define PIO_ISR 	0x4c	/* Interrupt Status Register        */ 
#define PIO_MDER 	0x50	/* Multi-driver Enable Register     */ 
#define PIO_MDDR 	0x54	/* Multi-driver Disable Register    */ 
#define PIO_MDSR 	0x58	/* Multi-driver Status Register     */ 
#define PIO_PUDR 	0x60	/* Pull-up Disable Register         */ 
#define PIO_PUER 	0x64	/* Pull-up Enable Register          */ 
#define PIO_PUSR 	0x68	/* Pad Pull-up Status Register      */ 
#define PIO_ASR 	0x70	/* Select A Register                */ 
#define PIO_BSR 	0x74	/* Select B Register                */ 
#define PIO_ABSR 	0x78	/* AB Select Status Register        */ 
#define PIO_OWER 	0xA0	/* Output Write Enable Register     */ 
#define PIO_OWDR 	0xA4	/* Output Write Disable Register    */ 
#define PIO_OWSR 	0xA8	/* Output Write Status Register     */ 
                                                                 
/* *****************************************************************************
 * Power Management and Clock Control (PMC)
 * *****************************************************************************/
#define PMC_BASE		0xFFFFFC00
#define PMC_REG(x)	*(volatile UINT32 *)(PMC_BASE + (x))

/* Register Offsets */
#define PMC_SCER 	0x00	/* System Clock Enable Register        */ 
#define PMC_SCDR 	0x04	/* System Clock Disable Register       */ 
#define PMC_SCSR 	0x08	/* System Clock Status Register        */ 
#define PMC_PCER 	0x10	/* Peripheral Clock Enable Register    */ 
#define PMC_PCDR 	0x14	/* Peripheral Clock Disable Register   */ 
#define PMC_PCSR 	0x18	/* Peripheral Clock Status Register    */ 
#define PMC_MOR 	0x20	/* Main Oscillator Register            */ 
#define PMC_MCFR 	0x24	/* Main Clock  Frequency Register      */ 
#define PMC_PLLAR 	0x28	/* PLL A Register                      */ 
#define PMC_PLLBR 	0x2C	/* PLL B Register                      */ 
#define PMC_MCKR 	0x30	/* Master Clock Register               */ 
#define PMC_PCKR0 	0x40	/* Programmable Clock Register 0       */ 
#define PMC_PCKR1 	0x44	/* Programmable Clock Register 1       */ 
#define PMC_PCKR2 	0x48	/* Programmable Clock Register 2       */ 
#define PMC_PCKR3 	0x4C	/* Programmable Clock Register 3       */ 
#define PMC_PCKR4 	0x50	/* Programmable Clock Register 4       */ 
#define PMC_PCKR5 	0x54	/* Programmable Clock Register 5       */ 
#define PMC_PCKR6 	0x58	/* Programmable Clock Register 6       */ 
#define PMC_PCKR7 	0x5C	/* Programmable Clock Register 7       */ 
#define PMC_IER 	0x60	/* Interrupt Enable Register           */ 
#define PMC_IDR 	0x64	/* Interrupt Disable Register          */ 
#define PMC_SR 		0x68	/* Status Register                     */ 
#define PMC_IMR 	0x6C	/* Interrupt Mask Register             */ 

/* -------- PMC_SCER : (PMC Offset: 0x0) System Clock Enable Register -------- */
#define PMC_PCK         BIT0  /* (PMC) Processor Clock                                            */
#define PMC_UDP         BIT1  /* (PMC) USB Device Port Clock                                      */
#define PMC_MCKUDP      BIT2  /* (PMC) USB Device Port Master Clock Automatic Disable on Suspend  */
#define PMC_UHP         BIT4  /* (PMC) USB Host Port Clock                                        */
#define PMC_PCK0        BIT8  /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK1        BIT9  /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK2        BIT10 /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK3        BIT11 /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK4        BIT12 /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK5        BIT13 /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK6        BIT14 /* (PMC) Programmable Clock Output                                  */
#define PMC_PCK7        BIT15 /* (PMC) Programmable Clock Output                                  */

/* -------- PMC_SCDR : (PMC Offset: 0x4) System Clock Disable Register -------- */

/* -------- PMC_SCSR : (PMC Offset: 0x8) System Clock Status Register -------- */

/* -------- PMC_MOR : (PMC Offset: 0x20) Main Oscillator Register -------- */
#define PMC_MOR_MOSCEN	  BIT0  /* (PMC) Main oscillator Enable */

/* -------- PMC_MCKR : (PMC Offset: 0x30) Master Clock Register -------- */
#define PMC_CSS           ((unsigned int) 0x3 <<  0) /* (PMC) Programmable Clock Selection             */
#define PMC_CSS_SLOW_CLK  ((unsigned int) 0x0) /* (PMC) Slow Clock is selected                         */
#define PMC_CSS_MAIN_CLK  ((unsigned int) 0x1) /* (PMC) Main Clock is selected                         */
#define PMC_CSS_PLLA_CLK  ((unsigned int) 0x2) /* (PMC) Clock from PLL A is selected                   */
#define PMC_CSS_PLLB_CLK  ((unsigned int) 0x3) /* (PMC) Clock from PLL B is selected                   */
#define PMC_PRES          ((unsigned int) 0x7 <<  2) /* (PMC) Programmable Clock Prescaler             */
#define PMC_PRES_CLK      ((unsigned int) 0x0 <<  2) /* (PMC) Selected clock                           */
#define PMC_PRES_CLK_2    ((unsigned int) 0x1 <<  2) /* (PMC) Selected clock divided by 2              */
#define PMC_PRES_CLK_4    ((unsigned int) 0x2 <<  2) /* (PMC) Selected clock divided by 4              */
#define PMC_PRES_CLK_8    ((unsigned int) 0x3 <<  2) /* (PMC) Selected clock divided by 8              */
#define PMC_PRES_CLK_16   ((unsigned int) 0x4 <<  2) /* (PMC) Selected clock divided by 16             */
#define PMC_PRES_CLK_32   ((unsigned int) 0x5 <<  2) /* (PMC) Selected clock divided by 32             */
#define PMC_PRES_CLK_64   ((unsigned int) 0x6 <<  2) /* (PMC) Selected clock divided by 64             */
#define PMC_MDIV          ((unsigned int) 0x3 <<  8) /* (PMC) Master Clock Division                    */
#define PMC_MDIV_1        ((unsigned int) 0x0 <<  8) /* (PMC) The master clock and the processor clock are the same           */
#define PMC_MDIV_2        ((unsigned int) 0x1 <<  8) /* (PMC) The processor clock is twice as fast as the master clock        */
#define PMC_MDIV_3        ((unsigned int) 0x2 <<  8) /* (PMC) The processor clock is three times faster than the master clock */
#define PMC_MDIV_4        ((unsigned int) 0x3 <<  8) /* (PMC) The processor clock is four times faster than the master clock  */

#define PMC_MCKR_PRES_MASK		0x0000001C				/* for masking out the PRES field */
#define PMC_MCKR_CSS_MASK		0x00000003				/* for masking out the CSS field  */

/* -------- PMC_PCKR : (PMC Offset: 0x40) Programmable Clock Register -------- */
#define PMC_PCKR_PRES_1			0x00				/* Peripheral Clock = CSS/1                     */ 
#define PMC_PCKR_PRES_2			0x04				/* Peripheral Clock = CSS/2                     */ 
#define PMC_PCKR_PRES_4			0x08				/* Peripheral Clock = CSS/4                     */ 
#define PMC_PCKR_PRES_8			0x1C				/* Peripheral Clock = CSS/8                     */ 
#define PMC_PCKR_PRES_16		0x10				/* Peripheral Clock = CSS/16                    */ 
#define PMC_PCKR_PRES_32		0x14				/* Peripheral Clock = CSS/32                    */ 
#define PMC_PCKR_PRES_64		0x18				/* Peripheral Clock = CSS/64                    */ 
#define PMC_PCKR_CSS_SLOW		0x00				/* Peripheral Clock Source = Slow Clock         */ 
#define PMC_PCKR_CSS_MAIN		0x01				/* Peripheral Clock Source = Main Oscillator    */ 
#define PMC_PCKR_CSS_PLLA		0x02  				/* Peripheral Clock Source = PLL A              */ 
#define PMC_PCKR_CSS_PLLB		0x03				/* Peripheral Clock Source = PLL B              */ 
                                                                                                         
/* -------- PMC_IER : (PMC Offset: 0x60) PMC Interrupt Enable Register -------- */
#define PMC_MOSCS       ((unsigned int) 0x1 <<  0) /* (PMC) MOSC Status/Enable/Disable/Mask     */
#define PMC_LOCKA       ((unsigned int) 0x1 <<  1) /* (PMC) PLL A Status/Enable/Disable/Mask    */
#define PMC_LOCKB       ((unsigned int) 0x1 <<  2) /* (PMC) PLL B Status/Enable/Disable/Mask    */
#define PMC_MCKRDY      ((unsigned int) 0x1 <<  3) /* (PMC) MCK_RDY Status/Enable/Disable/Mask  */
#define PMC_PCK0RDY     ((unsigned int) 0x1 <<  8) /* (PMC) PCK0_RDY Status/Enable/Disable/Mask */
#define PMC_PCK1RDY     ((unsigned int) 0x1 <<  9) /* (PMC) PCK1_RDY Status/Enable/Disable/Mask */
#define PMC_PCK2RDY     ((unsigned int) 0x1 << 10) /* (PMC) PCK2_RDY Status/Enable/Disable/Mask */
#define PMC_PCK3RDY     ((unsigned int) 0x1 << 11) /* (PMC) PCK3_RDY Status/Enable/Disable/Mask */
#define PMC_PCK4RDY     ((unsigned int) 0x1 << 12) /* (PMC) PCK4_RDY Status/Enable/Disable/Mask */
#define PMC_PCK5RDY     ((unsigned int) 0x1 << 13) /* (PMC) PCK5_RDY Status/Enable/Disable/Mask */
#define PMC_PCK6RDY     ((unsigned int) 0x1 << 14) /* (PMC) PCK6_RDY Status/Enable/Disable/Mask */
#define PMC_PCK7RDY     ((unsigned int) 0x1 << 15) /* (PMC) PCK7_RDY Status/Enable/Disable/Mask */

/* -------- PMC_IDR : (PMC Offset: 0x64) PMC Interrupt Disable Register -------- */

/* -------- PMC_SR : (PMC Offset: 0x68) PMC Status Register -------- */

/* -------- PMC_IMR : (PMC Offset: 0x6c) PMC Interrupt Mask Register -------- */
                                                                   
#define PMC_INT_PCK7_RDY		BIT15
#define PMC_INT_PCK6_RDY		BIT14
#define PMC_INT_PCK5_RDY		BIT13
#define PMC_INT_PCK4_RDY		BIT12
#define PMC_INT_PCK3_RDY		BIT11
#define PMC_INT_PCK2_RDY		BIT10
#define PMC_INT_PCK1_RDY		BIT9
#define PMC_INT_PCK0_RDY		BIT8
#define PMC_INT_MCK_RDY			BIT3
#define PMC_INT_LOCKB			BIT2
#define PMC_INT_LCKA			BIT1
#define PMC_INT_MOSCS			BIT0


/* *****************************************************************************
 * System timer (ST)
 * *****************************************************************************/
#define ST_BASE		0xFFFFFD00
#define ST_REG(x)	*(volatile UINT32 *)(ST_BASE + (x))

/* Register Offsets */
#define ST_CR 		0x00    /* Control Register                            */
#define ST_PIMR 	0x04    /* Period Interval Mode Register               */ 
#define ST_WDMR 	0x08    /* Watchdog Mode Register                      */ 
#define ST_RTMR 	0x0C    /* Real-time Mode Register                     */ 
#define ST_SR 		0x10    /* Status Register                             */ 
#define ST_IER 		0x14    /* Interrupt Enable Register                   */ 
#define ST_IDR 		0x18    /* Interrupt Disable Register                  */ 
#define ST_IMR 		0x1C    /* Interrupt Mask Register                     */ 
#define ST_RTAR 	0x20    /* Real-time Alarm Register                    */ 
#define ST_CRTR 	0x24    /* Current Real-time Register                  */ 
        
/* -------- ST_CR : (ST Offset: 0x0) System Timer Control Register -------- */
#define ST_WDRST        ((unsigned int) 0x1 <<  0)    /* (ST) Watchdog Timer Restart */

/* -------- ST_PIMR : (ST Offset: 0x4) System Timer Period Interval Mode Register -------- */ 
#define ST_PIV          ((unsigned int) 0xFFFF <<  0) /* (ST) Watchdog Timer Restart */

/* -------- ST_WDMR : (ST Offset: 0x8) System Timer Watchdog Mode Register -------- */
#define ST_WDV          ((unsigned int) 0xFFFF <<  0) /* (ST) Watchdog Timer Restart */
#define ST_RSTEN        ((unsigned int) 0x1 << 16)    /* (ST) Reset Enable */
#define ST_EXTEN        ((unsigned int) 0x1 << 17)    /* (ST) External Signal Assertion Enable */

/* -------- ST_RTMR : (ST Offset: 0xc) System Timer Real-time Mode Register -------- */
#define ST_RTPRES       ((unsigned int) 0xFFFF <<  0) /* (ST) Real-time Timer Prescaler Value */

/* -------- ST_SR : (ST Offset: 0x10) System Timer Status Register -------- */
#define ST_PITS         ((unsigned int) 0x1 <<  0)    /* (ST) Period Interval Timer Interrupt */
#define ST_WDOVF        ((unsigned int) 0x1 <<  1)    /* (ST) Watchdog Overflow               */
#define ST_RTTINC       ((unsigned int) 0x1 <<  2)    /* (ST) Real-time Timer Increment       */
#define ST_ALMS         ((unsigned int) 0x1 <<  3)    /* (ST) Alarm Status                    */

/* -------- ST_IER : (ST Offset: 0x14) System Timer Interrupt Enable Register -------- */

/* -------- ST_IDR : (ST Offset: 0x18) System Timer Interrupt Disable Register -------- */

/* -------- ST_IMR : (ST Offset: 0x1c) System Timer Interrupt Mask Register -------- */

/* -------- ST_RTAR : (ST Offset: 0x20) System Timer Real-time Alarm Register -------- */
#define ST_ALMV         ((unsigned int) 0xFFFFF <<  0) /* (ST) Alarm Value Value */

/* -------- ST_CRTR : (ST Offset: 0x24) System Timer Current Real-time Register -------- */
#define ST_CRTV         ((unsigned int) 0xFFFFF <<  0) /* (ST) Current Real-time Value */

                                                                    
/* *****************************************************************************
 * Real Time Clock (RTC)
 * *****************************************************************************/
#define RTC_BASE	0xFFFFFE00
#define RTC_REG(x)	*(volatile UINT32 *)(RTC_BASE + (x))

/* Register Offsets */
#define RTC_CR 		0x00    /* Control Register                            */ 
#define RTC_MR 		0x04    /* Mode Register                               */ 
#define RTC_TIMR 	0x08    /* Time Register                               */ 
#define RTC_CALR 	0x0C    /* Calendar Register                           */ 
#define RTC_TIMALR 	0x10    /* Time Alarm Register                         */ 
#define RTC_CALALR 	0x14    /* Calendar Alarm Register                     */ 
#define RTC_SR 		0x18    /* Status Register                             */ 
#define RTC_SCCR 	0x1C    /* Status Clear Command Register               */ 
#define RTC_IER 	0x20    /* Interrupt Enable Register                   */ 
#define RTC_IDR 	0x24    /* Interrupt Disable Register                  */ 
#define RTC_IMR 	0x28    /* Interrupt Mask Register                     */ 
#define RTC_VER 	0x2C    /* Valid Entry Register                        */ 
                                                                    
/* -------- RTC_CR : (RTC Offset: 0x0) RTC Control Register -------- */                                                  
#define RTC_UPDTIM            ((unsigned int) 0x1 <<  0) /* (RTC) Update Request Time Register                           */
#define RTC_UPDCAL            ((unsigned int) 0x1 <<  1) /* (RTC) Update Request Calendar Register                       */
#define RTC_TIMEVSEL          ((unsigned int) 0x3 <<  8) /* (RTC) Time Event Selection                                   */
#define RTC_TIMEVSEL_MINUTE   ((unsigned int) 0x0 <<  8) /* (RTC) Minute change.                                         */
#define RTC_TIMEVSEL_HOUR     ((unsigned int) 0x1 <<  8) /* (RTC) Hour change.                                           */
#define RTC_TIMEVSEL_DAY24    ((unsigned int) 0x2 <<  8) /* (RTC) Every day at midnight.                                 */
#define RTC_TIMEVSEL_DAY12    ((unsigned int) 0x3 <<  8) /* (RTC) Every day at noon.                                     */
#define RTC_CALEVSEL          ((unsigned int) 0x3 << 16) /* (RTC) Calendar Event Selection                               */
#define RTC_CALEVSEL_WEEK     ((unsigned int) 0x0 << 16) /* (RTC) Week change (every Monday at time 00:00:00).           */
#define RTC_CALEVSEL_MONTH    ((unsigned int) 0x1 << 16) /* (RTC) Month change (every 01 of each month at time 00:00:00).*/
#define RTC_CALEVSEL_YEAR     ((unsigned int) 0x2 << 16) /* (RTC) Year change (every January 1 at time 00:00:00).        */

/* -------- RTC_MR : (RTC Offset: 0x4) RTC Mode Register -------- */
#define RTC_HRMOD             ((unsigned int) 0x1 <<  0) /* (RTC) 12-24 hour Mode */

/* -------- RTC_TIMR : (RTC Offset: 0x8) RTC Time Register -------- */
#define RTC_SEC               ((unsigned int) 0x7F <<  0) /* (RTC) Current Second                           */
#define RTC_MIN               ((unsigned int) 0x7F <<  8) /* (RTC) Current Minute                           */
#define RTC_HOUR              ((unsigned int) 0x1F << 16) /* (RTC) Current Hour                             */
#define RTC_AMPM              ((unsigned int) 0x1 << 22)  /* (RTC) Ante Meridiem, Post Meridiem Indicator   */

/* -------- RTC_CALR : (RTC Offset: 0xc) RTC Calendar Register -------- */
#define RTC_CENT              ((unsigned int) 0x3F <<  0) /* (RTC) Current Century */
#define RTC_YEAR              ((unsigned int) 0xFF <<  8) /* (RTC) Current Year    */
#define RTC_MONTH             ((unsigned int) 0x1F << 16) /* (RTC) Current Month   */
#define RTC_DAY               ((unsigned int) 0x7 << 21)  /* (RTC) Current Day     */
#define RTC_DATE              ((unsigned int) 0x3F << 24) /* (RTC) Current Date    */

/* -------- RTC_TIMALR : (RTC Offset: 0x10) RTC Time Alarm Register -------- */
#define RTC_SECEN             ((unsigned int) 0x1 <<  7) /* (RTC) Second Alarm Enable */
#define RTC_MINEN             ((unsigned int) 0x1 << 15) /* (RTC) Minute Alarm        */
#define RTC_HOUREN            ((unsigned int) 0x1 << 23) /* (RTC) Current Hour        */

/* -------- RTC_CALALR : (RTC Offset: 0x14) RTC Calendar Alarm Register -------- */
#define RTC_MONTHEN           ((unsigned int) 0x1 << 23) /* (RTC) Month Alarm Enable     */
#define RTC_DATEEN            ((unsigned int) 0x1 << 31) /* (RTC) Date Alarm Enable      */

/* -------- RTC_SR : (RTC Offset: 0x18) RTC Status Register -------- */
#define RTC_ACKUPD            ((unsigned int) 0x1 <<  0) /* (RTC) Acknowledge for Update */
#define RTC_ALARM             ((unsigned int) 0x1 <<  1) /* (RTC) Alarm Flag             */
#define RTC_SECEV             ((unsigned int) 0x1 <<  2) /* (RTC) Second Event           */
#define RTC_TIMEV             ((unsigned int) 0x1 <<  3) /* (RTC) Time Event             */
#define RTC_CALEV             ((unsigned int) 0x1 <<  4) /* (RTC) Calendar event         */

/* -------- RTC_SCCR : (RTC Offset: 0x1c) RTC Status Clear Command Register -------- */

/* -------- RTC_IER : (RTC Offset: 0x20) RTC Interrupt Enable Register -------- */

/* -------- RTC_IDR : (RTC Offset: 0x24) RTC Interrupt Disable Register -------- */

/* -------- RTC_IMR : (RTC Offset: 0x28) RTC Interrupt Mask Register -------- */

/* -------- RTC_VER : (RTC Offset: 0x2c) RTC Valid Entry Register -------- */
#define RTC_NVTIM       ((unsigned int) 0x1 <<  0) /* (RTC) Non valid Time          */
#define RTC_NVCAL       ((unsigned int) 0x1 <<  1) /* (RTC) Non valid Calendar      */
#define RTC_NVTIMALR    ((unsigned int) 0x1 <<  2) /* (RTC) Non valid time Alarm    */
#define RTC_NVCALALR    ((unsigned int) 0x1 <<  3) /* (RTC) Nonvalid Calendar Alarm */

                                                                    
/* *****************************************************************************
 * Memory Controller (MC)
 * *****************************************************************************/
#define MC_BASE	    0xFFFFFF00
#define MC_REG(x)	*(volatile UINT32 *)(ST_BASE + (x))

/* Register Offsets */
#define MC_RCR 		0x00    /* MC Remap Control Register                   */ 
#define MC_ASR 		0x04    /* MC Abort Status Register                    */ 
#define MC_AASR 	0x08    /* MC Abort Address Status Register            */
#define MC_MPR 	    0x0C    /* MC Protection Unit Area                     */

/* -------- MC_RCR : (MC Offset: 0x0) MC Remap Control Register -------- */
#define MC_RCB            BIT0  /* (MC) Remap Command Bit */

/* -------- MC_ASR : (MC Offset: 0x4) MC Abort Status Register --------  */
#define MC_UNDADD         ((unsigned int) 0x1 <<  0)  /* (MC) Undefined Addess Abort Status        */
#define MC_MISADD         ((unsigned int) 0x1 <<  1)  /* (MC) Misaligned Addess Abort Status       */
#define MC_MPU            ((unsigned int) 0x1 <<  2)  /* (MC) Memory protection Unit Abort Status  */
#define MC_ABTSZ          ((unsigned int) 0x3 <<  8)  /* (MC) Abort Size Status                    */
#define MC_ABTSZ_BYTE     ((unsigned int) 0x0 <<  8)  /* (MC) Byte                                 */
#define MC_ABTSZ_HWORD    ((unsigned int) 0x1 <<  8)  /* (MC) Half-word                            */
#define MC_ABTSZ_WORD     ((unsigned int) 0x2 <<  8)  /* (MC) Word                                 */
#define MC_ABTTYP         ((unsigned int) 0x3 << 10)  /* (MC) Abort Type Status                    */
#define MC_ABTTYP_DATAR   ((unsigned int) 0x0 << 10)  /* (MC) Data Read                            */
#define MC_ABTTYP_DATAW   ((unsigned int) 0x1 << 10)  /* (MC) Data Write                           */
#define MC_ABTTYP_FETCH   ((unsigned int) 0x2 << 10)  /* (MC) Code Fetch                           */
#define MC_MST0           ((unsigned int) 0x1 << 16)  /* (MC) Master 0 Abort Source                */
#define MC_MST1           ((unsigned int) 0x1 << 17)  /* (MC) Master 1 Abort Source                */
#define MC_SVMST0         ((unsigned int) 0x1 << 24)  /* (MC) Saved Master 0 Abort Source          */
#define MC_SVMST1         ((unsigned int) 0x1 << 25)  /* (MC) Saved Master 1 Abort Source          */

/* -------- MC_PUIA : (MC Offset: 0x10) MC Protection Unit Area -------- */
#define MC_PROT           ((unsigned int) 0x3 <<  0)  /* (MC) Protection */
#define MC_PROT_PNAUNA    ((unsigned int) 0x0)        /* (MC) Privilege: No Access, User: No Access   */
#define MC_PROT_PRWUNA    ((unsigned int) 0x1)        /* (MC) Privilege: Read/Write, User: No Access  */
#define MC_PROT_PRWURO    ((unsigned int) 0x2)        /* (MC) Privilege: Read/Write, User: Read Only  */
#define MC_PROT_PRWURW    ((unsigned int) 0x3)        /* (MC) Privilege: Read/Write, User: Read/Write */
#define MC_SIZE           ((unsigned int) 0xF <<  4)  /* (MC) Internal Area Size                      */ 
#define MC_SIZE_1KB       ((unsigned int) 0x0 <<  4)  /* (MC) Area size 1KByte                        */ 
#define MC_SIZE_2KB       ((unsigned int) 0x1 <<  4)  /* (MC) Area size 2KByte                        */ 
#define MC_SIZE_4KB       ((unsigned int) 0x2 <<  4)  /* (MC) Area size 4KByte                        */ 
#define MC_SIZE_8KB       ((unsigned int) 0x3 <<  4)  /* (MC) Area size 8KByte                        */ 
#define MC_SIZE_16KB      ((unsigned int) 0x4 <<  4)  /* (MC) Area size 16KByte                       */ 
#define MC_SIZE_32KB      ((unsigned int) 0x5 <<  4)  /* (MC) Area size 32KByte                       */ 
#define MC_SIZE_64KB      ((unsigned int) 0x6 <<  4)  /* (MC) Area size 64KByte                       */ 
#define MC_SIZE_128KB     ((unsigned int) 0x7 <<  4)  /* (MC) Area size 128KByte                      */ 
#define MC_SIZE_256KB     ((unsigned int) 0x8 <<  4)  /* (MC) Area size 256KByte                      */ 
#define MC_SIZE_512KB     ((unsigned int) 0x9 <<  4)  /* (MC) Area size 512KByte                      */ 
#define MC_SIZE_1MB       ((unsigned int) 0xA <<  4)  /* (MC) Area size 1MByte                        */ 
#define MC_SIZE_2MB       ((unsigned int) 0xB <<  4)  /* (MC) Area size 2MByte                        */ 
#define MC_SIZE_4MB       ((unsigned int) 0xC <<  4)  /* (MC) Area size 4MByte                        */ 
#define MC_SIZE_8MB       ((unsigned int) 0xD <<  4)  /* (MC) Area size 8MByte                        */ 
#define MC_SIZE_16MB      ((unsigned int) 0xE <<  4)  /* (MC) Area size 16MByte                       */
#define MC_SIZE_64MB      ((unsigned int) 0xF <<  4)  /* (MC) Area size 64MByte                       */
#define MC_BA             ((unsigned int) 0x3FFFF << 10) /* (MC) Internal Area Base Address           */

/* -------- MC_PUP : (MC Offset: 0x50) MC Protection Unit Peripheral -------- */

/* -------- MC_PUER : (MC Offset: 0x54) MC Protection Unit Area -------- */
#define MC_PUEB           ((unsigned int) 0x1 <<  0)  /* (MC) Protection Unit enable Bit */



/* *****************************************************************************
 * Static Memory Interface Unit
 * *****************************************************************************/
#define SMC_BASE	        AT91C_BASE_SMC
#define SMC_REG(x)		    *(volatile UINT32 *)(SMC_BASE + (x))

/* registers offset */
#define SMC_CSR0 				0x00 	/* Chip Select Register 0 */ 
#define SMC_CSR1 				0x04 	/* Chip Select Register 1 */ 
#define SMC_CSR2 				0x08 	/* Chip Select Register 2 */ 
#define SMC_CSR3 				0x0C 	/* Chip Select Register 3 */ 
#define SMC_CSR4 				0x10 	/* Chip Select Register 4 */ 
#define SMC_CSR5 				0x14 	/* Chip Select Register 5 */
#define SMC_CSR6 				0x18 	/* Chip Select Register 6 */
#define SMC_CSR7 				0x1C 	/* Chip Select Register 7 */

/* -------- SMC2_CSR : (SMC2 Offset: 0x0) SMC2 Chip Select Register -------- */
#define SMC2_NWS              ((unsigned int) 0x7F <<  0) /* (SMC2) Number of Wait States     */
#define SMC2_WSEN             ((unsigned int) 0x1 <<  7) /* (SMC2) Wait State Enable          */
#define SMC2_TDF              ((unsigned int) 0xF <<  8) /* (SMC2) Data Float Time            */
#define SMC2_BAT              ((unsigned int) 0x1 << 12) /* (SMC2) Byte Access Type           */
#define SMC2_DBW              ((unsigned int) 0x1 << 13) /* (SMC2) Data Bus Width             */
#define SMC2_DBW_16           ((unsigned int) 0x1 << 13) /* (SMC2) 16-bit.                    */
#define SMC2_DBW_8            ((unsigned int) 0x2 << 13) /* (SMC2) 8-bit.                     */
#define SMC2_DRP              ((unsigned int) 0x1 << 15) /* (SMC2) Data Read Protocol         */
#define SMC2_ACSS             ((unsigned int) 0x3 << 16) /* (SMC2) Address to Chip Select Setup                                                */
#define SMC2_ACSS_STANDARD    ((unsigned int) 0x0 << 16) /* (SMC2) Standard, asserted at the beginning of the access and deasserted at the end.*/
#define SMC2_ACSS_1_CYCLE     ((unsigned int) 0x1 << 16) /* (SMC2) One cycle less at the beginning and the end of the access.                  */
#define SMC2_ACSS_2_CYCLES    ((unsigned int) 0x2 << 16) /* (SMC2) Two cycles less at the beginning and the end of the access.                 */
#define SMC2_ACSS_3_CYCLES    ((unsigned int) 0x3 << 16) /* (SMC2) Three cycles less at the beginning and the end of the access.               */
#define SMC2_RWSETUP          ((unsigned int) 0x7 << 24) /* (SMC2) Read and Write Signal Setup Time                                            */
#define SMC2_RWHOLD           ((unsigned int) 0x7 << 29) /* (SMC2) Read and Write Signal Hold Time                                             */

/* *****************************************************************************
 * Ethernet MAC (EMAC)
 * *****************************************************************************/
#define EMAC_BASE	    AT91C_BASE_EMAC
#define EMAC_REG(x)	    *(volatile UINT32 *)(EMAC_BASE + (x))

/* Register Offsets */
#define EMAC_CTL  0x00    /* Network Control Register              */ 
#define EMAC_CFG  0x04    /* Network Configuration Register        */ 
#define EMAC_SR   0x08    /* Network Status Register               */ 
#define EMAC_TAR  0x0C    /* Transmit Address Register             */ 
#define EMAC_TCR  0x10    /* Transmit Control Register             */ 
#define EMAC_TSR  0x14    /* Transmit Status Register              */ 
#define EMAC_RBQP 0x18    /* Receive Buffer Queue Pointer          */ 
#define EMAC_RSR  0x20    /* Receive Status Register               */ 
#define EMAC_ISR  0x24    /* Interrupt Status Register             */ 
#define EMAC_IER  0x28    /* Interrupt Enable Register             */ 
#define EMAC_IDR  0x2C    /* Interrupt Disable Register            */ 
#define EMAC_IMR  0x30    /* Interrupt Mask Register               */ 
#define EMAC_MAN  0x34    /* PHY Maintenance Register              */ 
                                                                    
#define EMAC_FRA  0x40    /* Frames Transmitted OK Register        */ 
#define EMAC_SCOL 0x44    /* Single Collision Frame Register       */ 
#define EMAC_MCOL 0x48    /* Multiple Collision Frame Register     */ 
#define EMAC_OK   0x4C    /* Frames Received OK Register           */ 
#define EMAC_SEQE 0x50    /* Frame Check Sequence Error Register   */ 
#define EMAC_ALE  0x54    /* Alignment Error Register              */ 
#define EMAC_DTE  0x58    /* Deferred Transmission Frame Register  */ 
#define EMAC_LCOL 0x5C    /* Late Collision Register               */ 
#define EMAC_ECOL 0x60    /* Excessive Collision Register          */ 
#define EMAC_CSE  0x64    /* Carrier Sense Error Register          */ 
#define EMAC_TUE  0x68    /* Transmit Underrun Error Register      */ 
#define EMAC_CDE  0x6C    /* Code Error Register                   */ 
#define EMAC_ELR  0x70    /* Excessive Length Error Register       */ 
#define EMAC_RJB  0x74    /* Receive Jabber Register               */ 
#define EMAC_USF  0x78    /* Undersize Frame Register              */ 
#define EMAC_SQEE 0x7C    /* SQE Test Error Register               */ 
#define EMAC_DRFC 0x80    /* Discarded RX Frame Register           */ 
                                                                    
#define EMAC_HSH  0x90    /* Hash Address High[63:32]              */      
#define EMAC_HSL  0x94    /* Hash Address Low[31:0]                */      
#define EMAC_SA1L 0x98    /* Specific Address 1 Low, First 4 bytes */      
#define EMAC_SA1H 0x9C    /* Specific Address 1 High, Last 2 bytes */      
#define EMAC_SA2L 0xA0    /* Specific Address 2 Low, First 4 bytes */      
#define EMAC_SA2H 0xA4    /* Specific Address 2 High, Last 2 bytes */      
#define EMAC_SA3L 0xA8    /* Specific Address 3 Low, First 4 bytes */      
#define EMAC_SA3H 0xAC    /* Specific Address 3 High, Last 2 bytes */      
#define EMAC_SA4L 0xB0    /* Specific Address 4 Low, First 4 bytes */     
#define EMAC_SA4H 0xB4    /* Specific Address 4 High, Last 2 bytes */    

/* *****************************************************************************
 * SDRAM Memory Interface Unit
 * *****************************************************************************/
#define SDRAMC_BASE	        AT91C_BASE_SDRAMC
#define SDRAMC_REG(x)		*(volatile UINT32 *)(SDRAMC_BASE + (x))

/* Registers */
#define SDRAMC_MR   				0x00 		/* Mode Register               */ 
#define SDRAMC_TR   				0x04 		/* Refresh Timer Register      */ 
#define SDRAMC_CR   				0x08 		/* Configuration Register      */ 
#define SDRAMC_SRR  				0x0C 		/* Self Refresh Register       */ 
#define SDRAMC_LPR  				0x10 		/* Low Power Register          */ 
#define SDRAMC_IER  				0x14 		/* Interrupt Enable Register   */ 
#define SDRAMC_IDR  				0x18 		/* Interrupt Disable Register  */ 
#define SDRAMC_IMR  				0x1C 		/* Interrupt Mask Register     */ 
#define SDRAMC_ISR  				0x20 		/* Interrupt Status Register   */

/* Bit Defines */
/* SDRC_MR - Mode Register */
#define SDRAMC_MR_DBW_16			BIT4	/* 1 = SDRAM is 16-bits wide, 0 = 32-bits                    */ 
#define SDRAMC_MR_NORM		        0x00	/* Normal Mode - All accesses to SDRAM are decoded normally  */ 
#define SDRAMC_MR_NOP				0x01	/* NOP Command is sent to SDRAM                              */ 
#define SDRAMC_MR_PRE				0x02	/* Precharge All Command is sent to SDRAM                    */ 
#define SDRAMC_MR_MRS				0x03	/* Mode Register Set Command is sent to SDRAM                */ 
#define SDRAMC_MR_REF				0x04	/* Refresh Command is sent to SDRAM                          */


/* *****************************************************************************
 * External Bus Interface Unit
 * *****************************************************************************/
#define EBI_CSA					0x00 	/* Chip Select Assignment Register */
#define EBI_CFGR				0x04 	/* Configuration Register */

/* Bit Defines */
/* EBI_CSA - Chip Select Assignment Register */
#define EBI_CSA_CS4_CF			BIT4	/* 1 = CS4-6 are assigned to Compact Flash, 0 = Chip Selects */
#define EBI_CSA_CS3_SMM			BIT3	/* 1 = CS3 is assigned to SmartMedia, 0 = Chip Select        */
#define EBI_CSA_CS1_SDRAM		BIT1	/* 1 = CS1 is assigned to SDRAM, 0 = Chip Select             */
#define EBI_CSA_CS0_BF			BIT0	/* 1 = CS0 is assigned to Burst Flash, 0 = Chip Select       */

/* EBI_CFGR	- Configuration Register */
#define EBI_CFGR_DBPU			BIT0	/* 1 = Disable D0-15 pullups */				


/* *****************************************************************************
 *               PERIPHERAL ID DEFINITIONS FOR AT91RM9200
 * *****************************************************************************/
#define AT91C_ID_FIQ    ((unsigned int)  0) /* Advanced Interrupt Controller (FIQ)    */ 
#define AT91C_ID_SYS    ((unsigned int)  1) /* System Peripheral                      */ 
#define AT91C_ID_PIOA   ((unsigned int)  2) /* Parallel IO Controller A               */ 
#define AT91C_ID_PIOB   ((unsigned int)  3) /* Parallel IO Controller B               */ 
#define AT91C_ID_PIOC   ((unsigned int)  4) /* Parallel IO Controller C               */ 
#define AT91C_ID_PIOD   ((unsigned int)  5) /* Parallel IO Controller D               */ 
#define AT91C_ID_US0    ((unsigned int)  6) /* USART 0                                */ 
#define AT91C_ID_US1    ((unsigned int)  7) /* USART 1                                */ 
#define AT91C_ID_US2    ((unsigned int)  8) /* USART 2                                */ 
#define AT91C_ID_US3    ((unsigned int)  9) /* USART 3                                */ 
#define AT91C_ID_MCI    ((unsigned int) 10) /* Multimedia Card Interface              */ 
#define AT91C_ID_UDP    ((unsigned int) 11) /* USB Device Port                        */ 
#define AT91C_ID_TWI    ((unsigned int) 12) /* Two-Wire Interface                     */ 
#define AT91C_ID_SPI    ((unsigned int) 13) /* Serial Peripheral Interface            */ 
#define AT91C_ID_SSC0   ((unsigned int) 14) /* Serial Synchronous Controller 0        */ 
#define AT91C_ID_SSC1   ((unsigned int) 15) /* Serial Synchronous Controller 1        */ 
#define AT91C_ID_SSC2   ((unsigned int) 16) /* Serial Synchronous Controller 2        */ 
#define AT91C_ID_TC0    ((unsigned int) 17) /* Timer Counter 0                        */ 
#define AT91C_ID_TC1    ((unsigned int) 18) /* Timer Counter 1                        */ 
#define AT91C_ID_TC2    ((unsigned int) 19) /* Timer Counter 2                        */ 
#define AT91C_ID_TC3    ((unsigned int) 20) /* Timer Counter 3                        */ 
#define AT91C_ID_TC4    ((unsigned int) 21) /* Timer Counter 4                        */ 
#define AT91C_ID_TC5    ((unsigned int) 22) /* Timer Counter 5                        */ 
#define AT91C_ID_UHP    ((unsigned int) 23) /* USB Host port                          */ 
#define AT91C_ID_EMAC   ((unsigned int) 24) /* Ethernet MAC                           */ 
#define AT91C_ID_IRQ0   ((unsigned int) 25) /* Advanced Interrupt Controller (IRQ0)   */ 
#define AT91C_ID_IRQ1   ((unsigned int) 26) /* Advanced Interrupt Controller (IRQ1)   */ 
#define AT91C_ID_IRQ2   ((unsigned int) 27) /* Advanced Interrupt Controller (IRQ2)   */ 
#define AT91C_ID_IRQ3   ((unsigned int) 28) /* Advanced Interrupt Controller (IRQ3)   */ 
#define AT91C_ID_IRQ4   ((unsigned int) 29) /* Advanced Interrupt Controller (IRQ4)   */ 
#define AT91C_ID_IRQ5   ((unsigned int) 30) /* Advanced Interrupt Controller (IRQ5)   */ 
#define AT91C_ID_IRQ6   ((unsigned int) 31) /* Advanced Interrupt Controller (IRQ6)   */ 
                                                                                      
                                                                                     
#endif  /* __INCat91rm9200h */
