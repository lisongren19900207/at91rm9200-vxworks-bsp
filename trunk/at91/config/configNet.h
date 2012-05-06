/* configNet.h - network configuration header */

/* Copyright 1999 Mistral Solutions Pvt Ltd */

/*
modification history
--------------------
2012/4/28 this file is modified form VxWorks demo bsp integrator920t

*/
 
#ifndef INCnetConfigh
#define INCnetConfigh

#ifdef __cplusplus
extern "C" {
#endif
	

#include "vxWorks.h"
#include "end.h"

#ifdef INCLUDE_END

#define AT91EMAC_LOAD_FUNC	at91EndLoad
#define	AT91EMAC_LOAN		1

/*
 * The initialization string format for Ethernet (unit = 0) is:
 *
 *  "<Speed>:<duplex>:<autoneg>"
 * Speed can be 10 or 100.  For duplex mode, 1 is for FDX and 0 for HDX
 * If autoneg is 1, autonegotiation is enabled, and speed and duplex mode are ignored
 *
 * The initialization string format for HDLC (unit = 1, 2) is:
 *
 *  "<channel>:<Baud Rate>:<dataFormat>"
 * Channel A = 0, Channel B = 1; NRZ = 0, NRZI = 1, FM0 = 2, FM1 = 3, and Manchester = 4
 */

#define AT91EMAC_LOAD_STRING  "100:1:1" 

IMPORT END_OBJ * AT91EMAC_LOAD_FUNC (char *, void *);

END_TBL_ENTRY endDevTbl [] =
{

#ifdef INCLUDE_AT91EMAC_END
    { 0, AT91EMAC_LOAD_FUNC, AT91EMAC_LOAD_STRING, AT91EMAC_LOAN , NULL, FALSE},
#endif	 /* INCLUDE_AT91END_END */
	/* End of table marker */
    { 0, END_TBL_END, NULL, 0, NULL, FALSE},
};

#endif  /*INCLUDE_END*/    

#ifdef __cplusplus
}
#endif

#endif /* INCnetConfigh */
