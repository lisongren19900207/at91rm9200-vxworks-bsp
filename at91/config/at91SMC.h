
#ifndef _AT91_SMC_DEFINE_HEADER
#define _AT91_SMC_DEFINE_HEADER

/*----------------------------MC--------------------*/           
#define	MC_BASE_ADDR 0xFFFFFF00	                                 
#define	MC_RCR_OFFSET	0                                        
#define	MC_ASR_OFFSET	0x4                                      
#define	MC_AASR_OFFSET	0x8                                      
#define	MC_MPR_OFFSET	0x0c                                     


/*-----------------------------SMC--------------------*/
#define	SMC_BASE_ADDR		(EBI_BASE_ADDR+0x10)

#define	SMC_CSR0_OFFSET	0

#define	SMC_CSR0_NWS	(0x0a)	/*for 120ns flash, 60M ,wait state should be 10	*/
#define	SMC_CSR0_WSEN	(0x01<<7)
#define	SMC_CSR0_TDF	(0x1<<8)
#define	SMC_CSR0_BAT	(0<<12)	
#define	SMC_CSR0_DBW	(0x1<<13)
#define	SMC_CSR0_DRP	(0x0<<15)
#define	SMC_CSR0_ACSS	(0x0<<16)
#define	SMC_CSR0_RWSETUP (0x0<<24)
#define	SMC_CSR0_RW_HOLD (0x0<<28)

#define	SMC_CSR0_VALUE	(SMC_CSR0_NWS|SMC_CSR0_WSEN|SMC_CSR0_TDF|SMC_CSR0_BAT|SMC_CSR0_DBW\
	|SMC_CSR0_DRP|SMC_CSR0_ACSS|SMC_CSR0_RWSETUP|SMC_CSR0_RW_HOLD)	

#endif	/*_AT91_SMC_DEFINE_HEADER*/

