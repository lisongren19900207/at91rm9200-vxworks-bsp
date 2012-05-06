
#ifndef _AT91_EMAC_DEFINE_HEADER
#define _AT91_EMAC_DEFINE_HEADER


#define EMAC_BASE_ADDR	0xFFFBC000

#ifndef _ASMLANGUAGE

typedef struct _ETH_STAT_S
{
	AT91_REG	 EMAC_FRA; 		/* Frames Transmitted OK Register           */ 
	AT91_REG	 EMAC_SCOL; 	/* Single Collision Frame Register          */
	AT91_REG	 EMAC_MCOL; 	/* Multiple Collision Frame Register        */
	AT91_REG	 EMAC_OK; 		/* Frames Received OK Register              */ 
	AT91_REG	 EMAC_SEQE; 	/* Frame Check Sequence Error Register      */
	AT91_REG	 EMAC_ALE; 		/* Alignment Error Register                 */ 
	AT91_REG	 EMAC_DTE; 		/* Deferred Transmission Frame Register     */ 
	AT91_REG	 EMAC_LCOL; 	/* Late Collision Register                  */
	AT91_REG	 EMAC_ECOL; 	/* Excessive Collision Register             */
	AT91_REG	 EMAC_CSE; 		/* Carrier Sense Error Register             */ 
	AT91_REG	 EMAC_TUE; 		/* Transmit Underrun Error Register         */ 
	AT91_REG	 EMAC_CDE; 		/* Code Error Register                      */ 
	AT91_REG	 EMAC_ELR; 		/* Excessive Length Error Register          */ 
	AT91_REG	 EMAC_RJB; 		/* Receive Jabber Register                  */ 
	AT91_REG	 EMAC_USF; 		/* Undersize Frame Register                 */ 
	AT91_REG	 EMAC_SQEE; 	/* SQE Test Error Register                  */
	AT91_REG	 EMAC_DRFC; 	/* Discarded RX Frame Register              */
}ETH_STAT_S;	


typedef struct _EMAC_S {                                             
	AT91_REG	 EMAC_CTL; 		/* Network Control Register            		*/      
	AT91_REG	 EMAC_CFG; 		/* Network Configuration Register           */ 
	AT91_REG	 EMAC_SR; 		/* Network Status Register                  */ 
	AT91_REG	 EMAC_TAR; 		/* Transmit Address Register                */ 
	AT91_REG	 EMAC_TCR; 		/* Transmit Control Register                */ 
	AT91_REG	 EMAC_TSR; 		/* Transmit Status Register                 */ 
	AT91_REG	 EMAC_RBQP; 	/* Receive Buffer Queue Pointer             */
	AT91_REG	 Reserved0[1]; 	/*                                          */
	AT91_REG	 EMAC_RSR; 		/* Receive Status Register                  */ 
	AT91_REG	 EMAC_ISR; 		/* Interrupt Status Register                */ 
	AT91_REG	 EMAC_IER; 		/* Interrupt Enable Register                */ 
	AT91_REG	 EMAC_IDR; 		/* Interrupt Disable Register               */ 
	AT91_REG	 EMAC_IMR; 		/* Interrupt Mask Register                  */ 
	AT91_REG	 EMAC_MAN; 		/* PHY Maintenance Register                 */ 
	AT91_REG	 Reserved1[2]; 	/*                                          */
	ETH_STAT_S	 ESTAT;
	AT91_REG	 Reserved2[3]; 	/*                                          */
	AT91_REG	 EMAC_HSH; 		/* Hash Address High[63:32]                 */ 
	AT91_REG	 EMAC_HSL; 		/* Hash Address Low[31:0]                   */ 
	AT91_REG	 EMAC_SA1L; 	/* Specific Address 1 Low, First 4 bytes    */
	AT91_REG	 EMAC_SA1H; 	/* Specific Address 1 High, Last 2 bytes    */
	AT91_REG	 EMAC_SA2L; 	/* Specific Address 2 Low, First 4 bytes    */
	AT91_REG	 EMAC_SA2H; 	/* Specific Address 2 High, Last 2 bytes    */
	AT91_REG	 EMAC_SA3L; 	/* Specific Address 3 Low, First 4 bytes    */
	AT91_REG	 EMAC_SA3H; 	/* Specific Address 3 High, Last 2 bytes    */
	AT91_REG	 EMAC_SA4L; 	/* Specific Address 4 Low, First 4 bytes    */
	AT91_REG	 EMAC_SA4H; 	/* Specific Address 4 High, Last 2 bytesr   */
} EMAC_S;



#endif 
 
#define EMAC_CTL_LB         ((unsigned int) 0x1 <<  0) 					/* (EMAC) Loopback. Optional. When set, loopback signal is at high level.*/ 
#define EMAC_CTL_LBL        ((unsigned int) 0x1 <<  1) 					/* (EMAC) Loopback local.                                                */
#define EMAC_CTL_RE         ((unsigned int) 0x1 <<  2) 					/* (EMAC) Receive enable.                                                */
#define EMAC_CTL_TE         ((unsigned int) 0x1 <<  3) 					/* (EMAC) Transmit enable.                                               */
#define EMAC_CTL_MPE        ((unsigned int) 0x1 <<  4) 					/* (EMAC) Management port enable.                                        */
#define EMAC_CTL_CSR        ((unsigned int) 0x1 <<  5) 					/* (EMAC) Clear statistics registers.                                    */
#define EMAC_CTL_ISR        ((unsigned int) 0x1 <<  6) 					/* (EMAC) Increment statistics registers.                                */
#define EMAC_CTL_WES        ((unsigned int) 0x1 <<  7) 					/* (EMAC) Write enable for statistics registers.                         */
#define EMAC_CTL_BP         ((unsigned int) 0x1 <<  8) 					/* (EMAC) Back pressure.                                                 */
/* -------- EMAC_CFG : (EMAC Offset: 0x4) Network Configuration Register --------  */
#define EMAC_CFG_SPD        ((unsigned int) 0x1 <<  0) 					/* (EMAC) Speed.  							*/
#define EMAC_CFG_FD         ((unsigned int) 0x1 <<  1) 					/* (EMAC) Full duplex.                      */
#define EMAC_CFG_BR         ((unsigned int) 0x1 <<  2) 					/* (EMAC) Bit rate.                         */
#define EMAC_CFG_CAF        ((unsigned int) 0x1 <<  4) 					/* (EMAC) Copy all frames.                  */
#define EMAC_CFG_NBC        ((unsigned int) 0x1 <<  5) 					/* (EMAC) No broadcast.                     */
#define EMAC_CFG_MTI        ((unsigned int) 0x1 <<  6) 					/* (EMAC) Multicast hash enable             */
#define EMAC_CFG_UNI        ((unsigned int) 0x1 <<  7) 					/* (EMAC) Unicast hash enable.              */
#define EMAC_CFG_BIG        ((unsigned int) 0x1 <<  8) 					/* (EMAC) Receive 1522 bytes.               */
#define EMAC_CFG_EAE        ((unsigned int) 0x1 <<  9) 					/* (EMAC) External address match enable.    */
#define EMAC_CFG_CLK_HCLK_8               ((unsigned int) 0x0 << 10) 	/* (EMAC) HCLK divided by 8                 */
#define EMAC_CFG_CLK_HCLK_16              ((unsigned int) 0x1 << 10) 	/* (EMAC) HCLK divided by 16                */
#define EMAC_CFG_CLK_HCLK_32              ((unsigned int) 0x2 << 10) 	/* (EMAC) HCLK divided by 32                */
#define EMAC_CFG_CLK_HCLK_64              ((unsigned int) 0x3 << 10) 	/* (EMAC) HCLK divided by 64                */
#define EMAC_CFG_RTY        ((unsigned int) 0x1 << 12) 					/* (EMAC)  */
#define EMAC_CFG_RMII       ((unsigned int) 0x1 << 13) 					/* (EMAC)  */
/* -------- EMAC_SR : (EMAC Offset: 0x8) Network Status Register --------  */
#define EMAC_SR_MDIO       ((unsigned int) 0x1 <<  1) 					/* (EMAC)  */
#define EMAC_SR_IDLE       ((unsigned int) 0x1 <<  2) 					/* (EMAC)  */
/* -------- EMAC_TCR : (EMAC Offset: 0x10) Transmit Control Register --------  */
#define EMAC_TCR_LEN        ((unsigned int) 0x7FF <<  0)   
#define EMAC_TCR_NCRC       ((unsigned int) 0x1 << 15)   
/* -------- EMAC_TSR : (EMAC Offset: 0x14) Transmit Control Register --------  */
#define EMAC_TSR_OVR        ((unsigned int) 0x1 <<  0)   
#define EMAC_TSR_COL        ((unsigned int) 0x1 <<  1)   
#define EMAC_TSR_RLE        ((unsigned int) 0x1 <<  2)   
#define EMAC_TSR_TXIDLE     ((unsigned int) 0x1 <<  3)   
#define EMAC_TSR_BNQ        ((unsigned int) 0x1 <<  4)   
#define EMAC_TSR_COMP       ((unsigned int) 0x1 <<  5)   
#define EMAC_TSR_UND        ((unsigned int) 0x1 <<  6)   
/* -------- EMAC_RSR : (EMAC Offset: 0x20) Receive Status Register --------  */
#define EMAC_RSR_BNA        ((unsigned int) 0x1 <<  0)   
#define EMAC_RSR_REC        ((unsigned int) 0x1 <<  1)  
#define EMAC_RSR_OVR        ((unsigned int) 0x1 <<  2)  
 
/* -------- EMAC_ISR : (EMAC Offset: 0x24) Interrupt Status Register --------  */
#define EMAC_ISR_DONE       ((unsigned int) 0x1 <<  0)   
#define EMAC_ISR_RCOM       ((unsigned int) 0x1 <<  1)   
#define EMAC_ISR_RBNA       ((unsigned int) 0x1 <<  2)   
#define EMAC_ISR_TOVR       ((unsigned int) 0x1 <<  3)   
#define EMAC_ISR_TUND       ((unsigned int) 0x1 <<  4)   
#define EMAC_ISR_RTRY       ((unsigned int) 0x1 <<  5)   
#define EMAC_ISR_TBRE       ((unsigned int) 0x1 <<  6)   
#define EMAC_ISR_TCOM       ((unsigned int) 0x1 <<  7)   
#define EMAC_ISR_TIDLE      ((unsigned int) 0x1 <<  8)   
#define EMAC_ISR_LINK       ((unsigned int) 0x1 <<  9)   
#define EMAC_ISR_ROVR       ((unsigned int) 0x1 << 10)   
#define EMAC_ISR_HRESP      ((unsigned int) 0x1 << 11)   
/* -------- EMAC_IER : (EMAC Offset: 0x28) Interrupt Enable Register --------  */
/* -------- EMAC_IDR : (EMAC Offset: 0x2c) Interrupt Disable Register -------- */ 
/* -------- EMAC_IMR : (EMAC Offset: 0x30) Interrupt Mask Register --------    */
/* -------- EMAC_MAN : (EMAC Offset: 0x34) PHY Maintenance Register --------   */
#define EMAC_PHY_DATA       ((unsigned int) 0xFFFF <<  0)   
#define EMAC_PHY_CODE       ((unsigned int) 0x3 << 16)  
#define EMAC_PHY_REGA       ((unsigned int) 0x1F << 18)  
#define EMAC_PHY_PHYA       ((unsigned int) 0x1F << 23)
  
#define EMAC_PHY_RW_R       ((unsigned int) 0x2 << 28)  
#define EMAC_PHY_RW_W       ((unsigned int) 0x1 << 28)  

#define EMAC_PHY_HIGH       ((unsigned int) 0x1 << 30)  
#define EMAC_PHY_LOW        ((unsigned int) 0x1 << 31)  

#define EMAC_RECV_DESC_WRAP	2
#define EMAC_RECV_DESC_HAVE_DATA  1


#endif /*_AT91_EMAC_DEFINE_HEADER*/

