
#ifndef _AT91_UHP_DEFINE_HEADER
#define _AT91_UHP_DEFINE_HEADER


#define UHP_BASE_ADDR 0x00300000

#ifndef _ASMLANGUAGE
typedef struct _UHP_S
{                                                                        
	AT91_REG	 UHP_HcRevision; 			/* Revision 											*/                                                   
	AT91_REG	 UHP_HcControl; 			/* Operating modes for the Host Controller              */       
	AT91_REG	 UHP_HcCommandStatus; 		/* Command & status Register                            */   
	AT91_REG	 UHP_HcInterruptStatus; 	/* Interrupt Status Register                            */
	AT91_REG	 UHP_HcInterruptEnable; 	/* Interrupt Enable Register                            */
	AT91_REG	 UHP_HcInterruptDisable; 	/* Interrupt Disable Register                           */
	AT91_REG	 UHP_HcHCCA; 				/* Pointer to the Host Controller Communication Area    */           
	AT91_REG	 UHP_HcPeriodCurrentED; 	/* Current Isochronous or Interrupt Endpoint Descriptor */
	AT91_REG	 UHP_HcControlHeadED; 		/* First Endpoint Descriptor of the Control list        */   
	AT91_REG	 UHP_HcControlCurrentED; 	/* Endpoint Control and Status Register                 */
	AT91_REG	 UHP_HcBulkHeadED; 			/* First endpoint register of the Bulk list             */       
	AT91_REG	 UHP_HcBulkCurrentED; 		/* Current endpoint of the Bulk list                    */   
	AT91_REG	 UHP_HcBulkDoneHead; 		/* Last completed transfer descriptor                   */   
	AT91_REG	 UHP_HcFmInterval; 			/* Bit time between 2 consecutive SOFs                  */       
	AT91_REG	 UHP_HcFmRemaining; 		/* Bit time remaining in the current Frame              */   
	AT91_REG	 UHP_HcFmNumber; 			/* Frame number                                         */       
	AT91_REG	 UHP_HcPeriodicStart; 		/* Periodic Start                                       */   
	AT91_REG	 UHP_HcLSThreshold; 		/* LS Threshold                                         */   
	AT91_REG	 UHP_HcRhDescriptorA; 		/* Root Hub characteristics A                           */   
	AT91_REG	 UHP_HcRhDescriptorB; 		/* Root Hub characteristics B                           */   
	AT91_REG	 UHP_HcRhStatus; 			/* Root Hub Status register                             */       
	AT91_REG	 UHP_HcRhPortStatus[2]; 	/* Root Hub Port Status Register                        */
}UHP_S;

#endif                                                                          

#endif /*_AT91_UHP_DEFINE_HEADER*/

