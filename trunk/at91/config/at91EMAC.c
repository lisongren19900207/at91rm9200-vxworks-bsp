/* at91EMAC.c - AT91RM9200 EMAC interface driver */

/* Copyright 1984-1997 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------

*/

/*
DESCRIPTION

*/

/* includes */

#include "vxWorks.h"
#include "etherMultiLib.h"		/* multicast stuff. */
#include "end.h"				/* Common END structures. */
#include "endLib.h"
#include "lstLib.h"				/* Needed to maintain protocol list. */
#include "cacheLib.h"
#include "stdlib.h"
#include "stdio.h"
#include "intLib.h"
#include "iv.h"
#include "netLib.h"
#include "config.h"
#include "at91Emac.h"			


#if defined(CPU_920T)
#include "arch/arm/mmuArmLib.h"
#include "private/vmLibP.h"
/*#include "dllLib.h"*/
#endif /* defined(720T/740T/920T/940T/946ES) */


/* defines */

/* Configuration items */
#define ENET_HDR_REAL_SIZ 	14
#define END_BUFSIZ      	( ETHERMTU + ENET_HDR_REAL_SIZ + 8 )
#define END_SPEED        	10000000
#define	AT91END_CL_SIZE		( ETHERMTU + ENET_HDR_REAL_SIZ + 8 )
#define END_MBLK_NUM			256
#define END_CL_NUM				(END_MBLK_NUM/2)


#define	RX_FD_NUM			32
#define	TX_FD_NUM			32

#define LS_POLLING          0x20

#define	AT91EMAC_MAX_MULTI	3

/*
 * Default macro definitions for BSP interface.
 * These macros can be redefined in a wrapper file, to generate
 * a new module with an optimized interface.
 */

/* Macro to connect interrupt handler to vector */



/* Macro to get the ethernet address from the BSP */

unsigned char at91EndEnetAddr[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};



#ifndef SYS_ENET_ADDR_GET
#   define SYS_ENET_ADDR_GET(pDevice) \
	{ \
	bcopy ((char *)at91EndEnetAddr, (char *)(&pDevice->enetAddr), 6); \
	}
#endif

/*
 * Macros to do a short (UINT16) access to the chip. Default
 * assumes a normal memory mapped device.
 */

/*A shortcut for getting the hardware address from the MIB II stuff. */

#define END_HADDR(pEnd)	\
		((pEnd)->mib2Tbl.ifPhysAddress.phyAddress)

#define END_HADDR_LEN(pEnd) \
		((pEnd)->mib2Tbl.ifPhysAddress.addrLength)

/* typedefs */

typedef struct _AT91_END_RECV_FD
{
	UINT32 bufaddr;
	UINT32 recvstatus;
}AT91_END_RECV_FD;


struct _AT91_END_TRAN_FD;

typedef struct _AT91_END_TRAN_FD
{
	char* bufaddr;
	UINT32 len;
	UINT32 owner;
}AT91_END_TRAN_FD;

typedef struct etherStatistics
{
	UINT32	 ESTAT_FRA; 	
	UINT32	 ESTAT_SCOL; 
	UINT32	 ESTAT_MCOL; 
	UINT32	 ESTAT_OK; 	
	UINT32	 ESTAT_SEQE; 
	UINT32	 ESTAT_ALE; 	
	UINT32	 ESTAT_DTE; 	
	UINT32	 ESTAT_LCOL; 
	UINT32	 ESTAT_ECOL; 
	UINT32	 ESTAT_CSE; 	
	UINT32	 ESTAT_TUE; 	
	UINT32	 ESTAT_CDE; 	
	UINT32	 ESTAT_ELR; 	
	UINT32	 ESTAT_RJB; 	
	UINT32	 ESTAT_USF; 	
	UINT32	 ESTAT_SQEE; 
	UINT32	 ESTAT_DRFC; 
} ETHER_STATISTICS;

#define  TXFIFOEMPTY	1
#define  TXFIFOFULL		2
#define  TXFIFONORMAL   3


/* The definition of the driver control structure */

/*
 *	for addrListLow and addrListHigh:
 *  ETH addr = E0 E1 E2 E3 E4 E5
 *	addrListLow( in byte address order ) = E3 E2 E1 E0
 *	addrListHigh( in byte address order ) = E5 E4
 *  so we can use htol and htos to convert the ETH address
 */
typedef struct _at91end_device
{
	END_OBJ    	end;			/* The class we inherit from. */
    int			unit;			/* unit number */
	int			level;		/* bdmaTx interrupt vector */
    long		flags;			/* Our local flags. */
	CL_POOL_ID  pClPoolId;
	UCHAR*		clPointBuf[ RX_FD_NUM ];

	UCHAR*		TranFifoBuf;
	UCHAR*		RecvFifoBuf;
	UINT32		TranPkgNumber;
	UINT32		TranPkgSize;
	AT91_END_TRAN_FD pTxBufStart[TX_FD_NUM];
	UINT32 TxPtr;		/* point to desc sending */
	UINT32 TxTailPtr;	/* point to desc may writing */
	UINT32 TXFifoStat;
	UINT32 NetJobDoing;

	AT91_END_RECV_FD* RecvBufDesc;
	UINT32		  rxBufPtr;

    	UCHAR		enetAddr[6];	/* ethernet address */
	UCHAR		netSpeed;		/* 10 or 100 */
	UCHAR		duplexMode;		/* HDX = 0. FDX = 1 */
	UCHAR		autoNeg;		/* 1 = autoneg enabled */
	BOOL		fdInitialized;	/* Set to TRUE after FD allocation */
	ETHER_STATISTICS	statistics;	/* Ethernet statistics counters */
	UINT32	addrListHigh[(AT91EMAC_MAX_MULTI+1)];	/* Array for storing addresses Max = 4, i.e. 32 long words */
	UINT32	addrListLow[(AT91EMAC_MAX_MULTI+1)];	/* Array for storing addresses Max = 4, i.e. 32 long words */
	UINT32	mcastAddrCount;	/* Number of valid multicast addresses */
} at91end_device;


/* globals*/

/*
 * This will only work if there is only a single unit, for multiple
 * unit device drivers these should be integrated into the at91end_device
 * structure.
 */

M_CL_CONFIG at91endMclConfig = 	/* network mbuf configuration table */
    {
    /* 
    no. mBlks		no. clBlks		memArea		memSize
    -----------		----------		-------		-------
    */
    0, 				0, 				NULL, 		0 
    };

CL_DESC at91endClDescTbl [] = 	/* network cluster pool configuration table */
    {
    /* 
    clusterSize		num		memArea		memSize
    -----------		----	-------		-------
    */
    {AT91END_CL_SIZE,	0,		NULL,		0}
    };

int endClDescTblNumEnt = (NELEMENTS(at91endClDescTbl));

/* new additions */


/* Definitions for the flags field */

#define END_PROMISCUOUS_FLAG	0x1
#define END_RCV_HANDLING_FLAG	0x2


/* DEBUG MACROS */

#undef  END_DEBUG /**/
/*

#define END_DEBUG
*/


#ifdef END_DEBUG
#include "LogLib.h"			/* Needed to debug polled mode.  */
int endDebug = 0;
#define ENDLOGMSG(x) \
	if (endDebug) \
	    { \
	    logMsg x;\
	    }
#else
#define ENDLOGMSG(x)
#endif /* END_DEBUG */

/* LOCALS */

#ifdef INCLUDE_AT91EMAC_END

/* forward static functions */

LOCAL void		at91EmacReset			(at91end_device *pDrvCtrl);    /**/
LOCAL void		at91EndInt				(at91end_device *pDrvCtrl);    /**/


LOCAL void 		at91EndMacInitialize	(at91end_device *pDevice);	/**/
LOCAL STATUS	at91EndFdInitialize		(at91end_device *pDrvCrtl); /**/
LOCAL void 		at91EndFdFree			(at91end_device *pDrvCtrl); /**/


LOCAL void		at91EndHandleRcvInt 	(at91end_device *pDrvCtrl, UINT32 stat);/**/
LOCAL STATUS	at91EndRecv				(at91end_device *pDrvCtrl, UINT32 recvdescptr );/**/
LOCAL void		at91EndConfig			(at91end_device *pDrvCtrl);/**/
LOCAL UINT32	at91EndPhyRead 			(UINT32 phyRegAddr, UINT32 phyAddr);/**/
LOCAL void 		at91EndPhyWrite 		(UINT32 phyRegAddr, UINT32 phyAddr, UINT32 phyData);/**/
LOCAL void		at91EndAddrFilterSet			(at91end_device *pDrvCtrl);/**/

/* END Specific interfaces. */

/* This is the only externally visible interface. */

END_OBJ* 	at91EndLoad (char* initString);

LOCAL STATUS	at91EndStart		(at91end_device* pDrvCtrl);/**/
LOCAL STATUS	at91EndStop			(at91end_device* pDrvCtrl);/**/
LOCAL STATUS	at91EndUnload		();/**/
LOCAL int		at91EndIoctl		(at91end_device* pDrvCtrl, int cmd, caddr_t data);
LOCAL STATUS	at91EndSend			(at91end_device* pDrvCtrl, M_BLK_ID pBuf);/**/
			  
LOCAL STATUS	at91EndMCastAdd 	(at91end_device* pDrvCtrl, char* pAddress);/**/
LOCAL STATUS	at91EndMCastDel 	(at91end_device* pDrvCtrl, char* pAddress);/**/
LOCAL STATUS	at91EndMCastGet 	(at91end_device* pDrvCtrl, MULTI_TABLE* pTable);/**/
LOCAL STATUS	at91EndPollSend 	(at91end_device* pDrvCtrl, M_BLK_ID pBuf);/**/
LOCAL STATUS	at91EndPollRcv 		(at91end_device* pDrvCtrl, M_BLK_ID pBuf);/**/
LOCAL STATUS	at91EndPollStart 	(at91end_device* pDrvCtrl);/**/
LOCAL STATUS	at91EndPollStop 	(at91end_device* pDrvCtrl);/**/

LOCAL STATUS	at91EndParse		();/*---*/
LOCAL STATUS	at91EndMemInit		();/*---*/
LOCAL void FreeEndMemory(  at91end_device* pDrvCtrl );

/*
 * Declare our function table.  This is static across all driver
 * instances.
 */

LOCAL NET_FUNCS at91endFuncTable =
    {
    (STATUS (*) (END_OBJ*))at91EndStart,				/* Function to start the device. */
    (STATUS (*) (END_OBJ*))at91EndStop,				/* Function to stop the device. */
    (STATUS (*) (END_OBJ*))at91EndUnload,				/* Unloading function for the driver. */
    (int (*) (END_OBJ*, int, caddr_t))at91EndIoctl,				/* Ioctl function for the driver. */
	(STATUS (*) (END_OBJ* , M_BLK_ID))at91EndSend,				/* Send function for the driver. */
    (STATUS (*) (END_OBJ*, char*))at91EndMCastAdd,			/* Multicast address add function for the driver. */
    (STATUS (*) (END_OBJ*, char*))at91EndMCastDel,			/* Multicast address delete function for the driver. */
    (STATUS (*) (END_OBJ*, MULTI_TABLE*))at91EndMCastGet,			/* Multicast table retrieve function for the driver. */
    (STATUS (*) (END_OBJ*, M_BLK_ID))at91EndPollSend,			/* Polling send function for the driver. */
    (STATUS (*) (END_OBJ*, M_BLK_ID))at91EndPollRcv,				/* Polling receive function for the driver. */
    endEtherAddressForm,        /* Put address info into a packet.  */
    endEtherPacketDataGet,      /* Get a pointer to packet data. */
    endEtherPacketAddrGet       /* Get packet addresses. */
    };

/*******************************************************************************
*
* at91EndLoad - initialize the driver and device
*
* This routine initializes the driver and the device to the operational state.
* All of the device specific parameters are passed in the initString.
*
* The string contains the target specific parameters like this:
*
* "<unit>:<Speed>:<duplex>:<autoneg>"
*
* RETURNS: An END object pointer or NULL on error.
*/

END_OBJ* at91EndLoad
    (
    char* initString		/* String to be parsed by the driver. */
    )
{
    at91end_device 	*pDrvCtrl;

    ENDLOGMSG (("Loading at91EndLoad...\n", 1, 2, 3, 4, 5, 6));

	if ( initString[0] == 0 )
	{
		strcpy (initString, "atemac");
		return (END_OBJ *)NULL;
	}

	/* allocate the device structure */
    pDrvCtrl = (at91end_device *)calloc (sizeof (at91end_device), 1);
    if ( pDrvCtrl == NULL )
		goto errorExit;
	
	bzero( (void*)pDrvCtrl, sizeof (at91end_device) );
	pDrvCtrl ->TXFifoStat = TXFIFOEMPTY;

    /* parse the init string, filling in the device structure */
    if ( at91EndParse ( pDrvCtrl, initString ) == ERROR )
		goto errorExit;

    pDrvCtrl->level = INT_LVL_EMAC;

    /* Ask the BSP to provide the ethernet address. */
    SYS_ENET_ADDR_GET( pDrvCtrl );

    /* initialize the END and MIB2 parts of the structure */
	strcpy (pDrvCtrl->end.devObject.name, "atemac");
	strcpy (pDrvCtrl->end.devObject.description, "AT91RM9200 EMAC END Driver");
	

    /*
     * The M2 element must come from m2Lib.h 
     * This at91End is set up for a DIX type ethernet device.
     */
    if ( END_OBJ_INIT ( &pDrvCtrl->end, (DEV_OBJ *)pDrvCtrl, "atemac",
                      pDrvCtrl->unit, &at91endFuncTable,
                      "AT91RM9200 EMAC END Driver" ) == ERROR
     || END_MIB_INIT (&pDrvCtrl->end, M2_ifType_ethernet_csmacd,
                      &pDrvCtrl->enetAddr[0], 6, ETHERMTU,
                      END_SPEED) == ERROR )
		goto errorExit;

    /* Perform memory allocation/distribution */
    if ( at91EndMemInit (pDrvCtrl) == ERROR )
		goto errorExit;

    /* reset and reconfigure the device */
    at91EndConfig (pDrvCtrl);

    /* set the flags to indicate readiness */
    END_OBJ_READY ( &pDrvCtrl->end, IFF_NOTRAILERS | IFF_BROADCAST | IFF_MULTICAST );
	ENDLOGMSG (("Loading at91EndLoad success EXIT...\n", 1, 2, 3, 4, 5, 6));

	return ( &pDrvCtrl->end );

errorExit:
    if ( pDrvCtrl != NULL )
	{
		FreeEndMemory( pDrvCtrl );
		pDrvCtrl = NULL;
	}
	ENDLOGMSG (("Loading at91EndLoad Failure EXIT...\n", 1, 2, 3, 4, 5, 6));
    return NULL;
}

/*******************************************************************************
*
* at91EndParse - parse the init string
*
* Parse the input string.  Fill in values in the driver control structure.
*
* The initialization string format is:
* "<unit>:<Speed>:<duplex>:<autoneg>"
*
* .bS
* unit			Device unit number, a small integer.
* Speed			10 (10Mbps) or 100 (100 Mbps)
* duplex		0 (HDX) or 1 (FDX)
* autoneg		Autonegotiation disabled (0) or enabled (1)
* .bE
*
* RETURNS: OK or ERROR for invalid arguments.
*/

STATUS at91EndParse
(
	at91end_device * pDrvCtrl,	/* device pointer */
	char * initString		/* information string */
)
{
    char*	tok;
    char*	pHolder = NULL;
    
    /* Parse the initString */
	
	/* Unit number. */
    tok = strtok_r (initString, ":", &pHolder);
    if (tok == NULL)
		return ERROR;
    pDrvCtrl->unit = atoi (tok);
	
	/* netSpeed */
    tok = strtok_r (NULL, ":", &pHolder);
    if (tok == NULL)
		return ERROR;
    pDrvCtrl->netSpeed = atoi (tok);
	
	
	/* DuplexMode */
    tok = strtok_r (NULL, ":", &pHolder);
    if (tok == NULL)
		return ERROR;
    pDrvCtrl->duplexMode = atoi (tok);
	
	/* auto Negotiation */
    tok = strtok_r (NULL, ":", &pHolder);
    if (tok == NULL)
		return ERROR;
    pDrvCtrl->autoNeg = atoi (tok);
	
    return OK;
}

/*
 *	format AT91 Special address REG word
 */

LOCAL void FormatETHSpecialAddrWord( UINT32 *wordHigh, UINT32 *wordLow, BYTE *macadd )
{
	(*wordLow) = /*htonl( *((UINT32*)( pDevice->enetAddr )) );*/
		((UINT32)(macadd[3])<<24) | ((UINT32)(macadd[2])<<16) | ((UINT32)(macadd[1])<<8) | ((UINT32)(macadd[0]));
	(*wordHigh) = /*htons( *((UINT16*)( pDevice->enetAddr + 4 )) )*/
		((UINT32)(macadd[5])<<8) | ((UINT32)(macadd[4])) ;
	
}




/*******************************************************************************
*
* at91EndMemInit - initialize memory for the chip
*
* This routine is highly specific to the device.  
*
* RETURNS: OK or ERROR.
*/

STATUS at91EndMemInit
    (
    at91end_device * pDrvCtrl	/* device to be initialized */
    )
{

    UINT32 recvfdsize;
	UINT32 txbufsize;
	UINT32 rxbufsize;
	UINT32 tmp;
	/*
     * This is how we would set up and END netPool using netBufLib(1).
     * This code is pretty generic.
     */
    if ( (pDrvCtrl->end.pNetPool = (NET_POOL_ID) malloc (sizeof(NET_POOL))) == NULL )
        return (ERROR);

	/*
	 *	allocate the Trans desc buffer
	 */
    at91endMclConfig.mBlkNum =  END_MBLK_NUM;
    at91endClDescTbl[0].clNum = END_CL_NUM;
    at91endMclConfig.clBlkNum = at91endClDescTbl[0].clNum;

    /* Calculate the total memory for all the M-Blks and CL-Blks. */
    at91endMclConfig.memSize = 
		(at91endMclConfig.mBlkNum * (MSIZE + sizeof (long))) +
        (at91endMclConfig.clBlkNum * (CL_BLK_SZ + sizeof(long)));

	at91endMclConfig.memArea = 
		(char *) memalign (sizeof(UINT32), at91endMclConfig.memSize );
    if ( at91endMclConfig.memArea == NULL)
	{
		ENDLOGMSG(( " allocate m and cl block memory failure ", 1, 2, 3, 4, 5, 6 ));
        return (ERROR);
	}

	at91endClDescTbl[0].clSize = ROUND_UP( at91endClDescTbl[0].clSize, 4 );
    
    /* Calculate the memory size of all the clusters. */
    at91endClDescTbl[0].memSize = 
		ROUND_UP( (at91endClDescTbl[0].clNum) * (at91endClDescTbl[0].clSize+4)+sizeof(int), _CACHE_ALIGN_SIZE );
	/*
	 *	allocate Received Buffer Descriptor List memory space, this also should be 
	 *	none cached
	 sizeof(AT91_END_RECV_FD) )*2 is for align

	 */
	recvfdsize = ROUND_UP( RX_FD_NUM*( sizeof(AT91_END_RECV_FD) )*2, _CACHE_ALIGN_SIZE );
	rxbufsize  = ROUND_UP( RX_FD_NUM*at91endClDescTbl[0].clSize, _CACHE_ALIGN_SIZE );

	/*
	 *	allocate Tx fifo buffer space 
	 *	each block size same as receive buffer
	 */
	txbufsize = ROUND_UP( TX_FD_NUM*at91endClDescTbl[0].clSize, _CACHE_ALIGN_SIZE );

    /* Allocate the memory for the clusters , tx buffer and recv descriptor
	 * each memory is insert with a _CACHE_ALIGN_SIZE space for ascess safety
	 */
	at91endClDescTbl[0].memArea = (char*)malloc( at91endClDescTbl[0].memSize + _CACHE_ALIGN_SIZE );
/*
		= (char *) cacheDmaMalloc ( at91endClDescTbl[0].memSize + txbufsize + recvfdsize + _CACHE_ALIGN_SIZE*3 );
*/

    if ( at91endClDescTbl[0].memArea == NULL )
	{
		ENDLOGMSG(( " allocate none cached clusters memory failure ", 1, 2, 3, 4, 5, 6 ));
        return (ERROR);
	}

	/*
	 *	save the points
	 */
	pDrvCtrl ->TranFifoBuf = (char*)cacheDmaMalloc( txbufsize + recvfdsize + rxbufsize + _CACHE_ALIGN_SIZE*3 );
	if( pDrvCtrl ->TranFifoBuf == NULL )
	{
		ENDLOGMSG(( " allocate none cached recv and transmit failure ", 1, 2, 3, 4, 5, 6 ));
        return (ERROR);
	}
	
/*
	pDrvCtrl ->TranFifoBuf = 
		at91endClDescTbl[0].memArea + at91endClDescTbl[0].memSize + _CACHE_ALIGN_SIZE;
*/

	pDrvCtrl ->RecvFifoBuf = (char*)( pDrvCtrl ->TranFifoBuf + txbufsize + _CACHE_ALIGN_SIZE  );

	tmp = (UINT32)( (UINT32)(pDrvCtrl ->RecvFifoBuf) + rxbufsize + _CACHE_ALIGN_SIZE + (RX_FD_NUM*( sizeof(AT91_END_RECV_FD) )) -1 );
	tmp &= (~( (RX_FD_NUM*( sizeof(AT91_END_RECV_FD) )) -1) );

	pDrvCtrl ->RecvBufDesc = (AT91_END_RECV_FD*)( tmp );
	
	pDrvCtrl ->TranPkgNumber = TX_FD_NUM;
	pDrvCtrl ->TranPkgSize = at91endClDescTbl[0].clSize;
	bzero( (void*)(pDrvCtrl ->RecvBufDesc), RX_FD_NUM*sizeof(AT91_END_RECV_FD)  );


    /* Initialize the memory pool. */
    if ( netPoolInit(pDrvCtrl->end.pNetPool, &at91endMclConfig,
                    &at91endClDescTbl[0], endClDescTblNumEnt, NULL ) == ERROR )
    {
		ENDLOGMSG(( " initial net pool for at91 END failure ", 1, 2, 3, 4, 5, 6 ));
        return (ERROR);
    }

    pDrvCtrl->pClPoolId = clPoolIdGet ( pDrvCtrl->end.pNetPool,
		at91endClDescTbl[0].clSize, FALSE);
	
    return OK;
}
/*******************************************************************************
*
* at91EndStart - start the device
*
* This function calls BSP functions to connect interrupts and start the
* device running in interrupt mode.
*
* RETURNS: OK or ERROR
*
*/

LOCAL STATUS at91EndStart
    (
    at91end_device *pDrvCtrl	/* device to be started */
    )
{


	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	UINT32 dummy;

	ENDLOGMSG ((" Enter at91EndStart...\n", 1, 2, 3, 4, 5, 6));	

	/*
	 *	load control register: for back pressure we don't use, start here ?
	 */
	pemac ->EMAC_CTL = EMAC_CTL_RE | EMAC_CTL_TE ;

	pemac ->EMAC_IDR = 0xFFFFFFFF;
	dummy = pemac ->EMAC_ISR;
	
	/* Connect BDMA and MAC TX and RX interrupts */ /*modified by knp/nts 27/9/99 */ 												
	intConnect ( (VOIDFUNCPTR *)INUM_TO_IVEC ( pDrvCtrl->level ), at91EndInt, (UINT32) pDrvCtrl );

	/* Enable all the four interrupts */ /*modified by knp/nts 27/9/9/99 */
	intEnable ( pDrvCtrl->level );

	
	/*
	 *	enable interrupt if not in polling mode.
	 */
	if ( (pDrvCtrl->flags & LS_POLLING) == 0 )
		pemac ->EMAC_IER = 	
		pemac ->EMAC_IER = 	EMAC_ISR_RBNA /*| EMAC_ISR_TOVR*/ | EMAC_ISR_TUND | EMAC_ISR_RTRY
			/*| EMAC_ISR_TBRE */| EMAC_ISR_TCOM | EMAC_ISR_ROVR | EMAC_ISR_HRESP | EMAC_ISR_RCOM;
		
	
	
	/* Set the flags to indicate that the device is up */
	END_FLAGS_SET (&pDrvCtrl->end, IFF_UP | IFF_RUNNING);

	ENDLOGMSG ((" leave at91EndStart...\n", 1, 2, 3, 4, 5, 6));

	
    return (OK);
}



/*******************************************************************************
* at91EndBdmaRxInt - handle controller interrupt
*
* This routine is called at interrupt level in response to an interrupt from
* the BdmaTx controller.
*
* RETURNS: N/A.
*/

LOCAL void TranNextPackage( at91end_device* pDrvCtrl )
{
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;	

	if( 
		  ( (pDrvCtrl ->TxPtr) != (pDrvCtrl ->TxTailPtr) )
			|| ( pDrvCtrl ->TXFifoStat == TXFIFOFULL )  
	  )
	{

		ENDLOGMSG ((" END:Interrupt pack sended ...\n", 1, 2, 3, 4, 5, 6));

		pemac ->EMAC_TAR = (UINT32)((pDrvCtrl ->pTxBufStart[pDrvCtrl ->TxPtr]).bufaddr);
		/*
		 *	write a length start the tans
		 */
		pemac ->EMAC_TCR = ( pDrvCtrl ->pTxBufStart[pDrvCtrl ->TxPtr]).len&(0x7FF) ;
		
		if( pDrvCtrl ->TxPtr == (TX_FD_NUM - 1) )
			pDrvCtrl ->TxPtr = 0;
		else
			pDrvCtrl ->TxPtr ++;
			
		if( pDrvCtrl ->TxPtr == pDrvCtrl ->TxTailPtr )
		{
			pDrvCtrl ->TXFifoStat = TXFIFOEMPTY;
		}
		else
			pDrvCtrl ->TXFifoStat = TXFIFONORMAL;
	}
}


LOCAL void at91EndInt
    (
    at91end_device  *pDrvCtrl	/* interrupting device */
    )
{
	UINT32 status, txstatus;
	EMAC_S* pemac = (EMAC_S*)(EMAC_BASE_ADDR);
	char *nextp;


	/*
	 *	read the Int status register, clear ISR status at same time
	 */
	status = pemac ->EMAC_ISR;

	if( status & 
		( EMAC_ISR_RBNA | EMAC_ISR_TOVR | EMAC_ISR_TUND | EMAC_ISR_TBRE 
			| EMAC_ISR_ROVR | EMAC_ISR_HRESP  ) )
	{
		ENDLOGMSG( ("END: error occured, interrupt status reg is %x", status, 2,3,4,5,6) );
	}

	/*
	 *	avoid dead lock of receive buf if not available
	 */
/*
	if( status & EMAC_ISR_RBNA )
	{
		pemac ->EMAC_CTL &= ( ~EMAC_CTL_RE );
		pemac ->EMAC_CTL |= EMAC_CTL_RE;
	}
*/

	/*
	 *	reflash error counter
	 */
	pDrvCtrl ->statistics.ESTAT_FRA += pemac ->ESTAT.EMAC_FRA;
	pDrvCtrl ->statistics.ESTAT_SCOL+= pemac ->ESTAT.EMAC_SCOL;  
	pDrvCtrl ->statistics.ESTAT_MCOL+= pemac ->ESTAT.EMAC_MCOL;  
	pDrvCtrl ->statistics.ESTAT_OK 	+= pemac ->ESTAT.EMAC_OK; 	
	pDrvCtrl ->statistics.ESTAT_SEQE+= pemac ->ESTAT.EMAC_SEQE;  
	pDrvCtrl ->statistics.ESTAT_ALE	+= pemac ->ESTAT.EMAC_ALE;   
	pDrvCtrl ->statistics.ESTAT_DTE	+= pemac ->ESTAT.EMAC_DTE;   
	pDrvCtrl ->statistics.ESTAT_LCOL+= pemac ->ESTAT.EMAC_LCOL;  
	pDrvCtrl ->statistics.ESTAT_ECOL+= pemac ->ESTAT.EMAC_ECOL;  
	pDrvCtrl ->statistics.ESTAT_CSE += pemac ->ESTAT.EMAC_CSE;   
	pDrvCtrl ->statistics.ESTAT_TUE	+= pemac ->ESTAT.EMAC_TUE;   
	pDrvCtrl ->statistics.ESTAT_CDE += pemac ->ESTAT.EMAC_CDE;   
	pDrvCtrl ->statistics.ESTAT_ELR += pemac ->ESTAT.EMAC_ELR;   
	pDrvCtrl ->statistics.ESTAT_RJB	+= pemac ->ESTAT.EMAC_RJB;   
	pDrvCtrl ->statistics.ESTAT_USF += pemac ->ESTAT.EMAC_USF;   
	pDrvCtrl ->statistics.ESTAT_SQEE+= pemac ->ESTAT.EMAC_SQEE;  
	pDrvCtrl ->statistics.ESTAT_DRFC+= pemac ->ESTAT.EMAC_DRFC;  

	/*
	 *	do receive job
	 */
	if( (pDrvCtrl ->NetJobDoing == FALSE) && ( status & EMAC_ISR_RCOM ) )
	{
		ENDLOGMSG ((" END:Interrupt pack received ...\n", 1, 2, 3, 4, 5, 6));

		netJobAdd ((FUNCPTR)at91EndHandleRcvInt, (int)pDrvCtrl, status, 0 , 0 , 0 );
		pDrvCtrl ->NetJobDoing = TRUE;
	}

	/*
	 *	do transmit job
	 */
	txstatus = pemac ->EMAC_TSR;

	if(  /*(status&EMAC_ISR_TCOM )&&*/( txstatus & EMAC_TSR_BNQ ) )
	{
		/*
		 *	TxPtr ->next ready to send package
		 *	TxTailPtr ->Next to accept transmit package
		 */		 
		TranNextPackage( pDrvCtrl );
	}
}



/*******************************************************************************
*
* at91EndHandleRcvInt - task level interrupt service for input packets
*
* This routine is called at task level indirectly by the interrupt
* service routine to do any message received processing.
*
* RETURNS: N/A.

*/


LOCAL void at91EndHandleRcvInt
    (
    at91end_device *pDrvCtrl,	/* interrupting device */
	UINT32 stat	/* receive status */
    )
{
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	AT91_END_RECV_FD* startadd = pDrvCtrl->RecvBufDesc;

	ENDLOGMSG ((" Enter at91EndHandleRcvInt routine...\n", 1, 2, 3, 4, 5, 6));

	pDrvCtrl ->NetJobDoing = FALSE;

	/*startadd[pDrvCtrl ->rxBufPtr].bufaddr+=2;*/

	ENDLOGMSG ((" [0x%x].bufaddr= 0x%x .recvstatus= 0x%x\n",pDrvCtrl ->rxBufPtr,startadd[pDrvCtrl ->rxBufPtr].bufaddr,startadd[pDrvCtrl ->rxBufPtr].recvstatus, 4, 5, 6));
	while( (startadd[pDrvCtrl ->rxBufPtr].bufaddr) & EMAC_RECV_DESC_HAVE_DATA	 )
	{
		at91EndRecv ( pDrvCtrl, pDrvCtrl ->rxBufPtr );
		
/*
		if( pDrvCtrl ->rxBufPtr == (RX_FD_NUM -1) )
		{
			pDrvCtrl ->rxBufPtr = 0;
		}
		else
			pDrvCtrl ->rxBufPtr ++;
*/
	}
	ENDLOGMSG ((" Leave at91EndHandleRcvInt routine...\n", 1, 2, 3, 4, 5, 6));
}


/*******************************************************************************
*
* at91EndRecv - process the next incoming packet
*
* Handle one incoming packet.  The packet is checked for errors.
*
* RETURNS: N/A.
*/

LOCAL STATUS at91EndRecv
    (
    at91end_device *pDrvCtrl,	/* device structure */
	UINT32 recvdescptr
    )
{
    M_BLK_ID 	pMblk;
    char*       pNewCluster;
    CL_BLK_ID	pClBlk;
	UINT32 tmp;
	UINT32 len;

	ENDLOGMSG( ( "END: Enter at91EndRecv *******.\n", 1,2,3,4,5,6) );

    /* Add one to our unicast data. */
	END_ERR_ADD (&pDrvCtrl->end, MIB2_IN_UCAST, +1);

    /*
     * We implicitly are loaning here, if copying is necessary this
     * step may be skipped, but the data must be copied before being
     * passed up to the protocols.
     */
    pNewCluster = netClusterGet (pDrvCtrl->end.pNetPool, pDrvCtrl->pClPoolId );

    if (pNewCluster == NULL)
    {
		END_ERR_ADD (&pDrvCtrl->end, MIB2_IN_ERRS, +1);
		goto cleanRXD;
    }

	if( ((UINT32)pNewCluster)&3 )
	{
		ENDLOGMSG(( "END:handle recv get clust buffer not word aligned",(UINT32)pNewCluster,2,3,4,5,6  ));
	}

    /* Grab a cluster block to marry to the cluster we received. */
    if ( (pClBlk = netClBlkGet (pDrvCtrl->end.pNetPool, M_DONTWAIT) ) == NULL)
    {
        netClFree ( pDrvCtrl->end.pNetPool, pNewCluster );
		END_ERR_ADD ( &pDrvCtrl->end, MIB2_IN_ERRS, +1 );
		goto cleanRXD;
    }


    /*
     * OK we've got a spare, let's get an M_BLK_ID and marry it to the
     * one in the ring.
     */
    if ((pMblk = mBlkGet (pDrvCtrl->end.pNetPool, M_DONTWAIT, MT_DATA)) == NULL)
    {
        netClBlkFree ( pDrvCtrl->end.pNetPool, pClBlk ); 
        netClFree ( pDrvCtrl->end.pNetPool, pNewCluster );
		END_ERR_ADD ( &pDrvCtrl->end, MIB2_IN_ERRS, +1 );
		goto cleanRXD;
    }

    END_ERR_ADD (&pDrvCtrl->end, MIB2_IN_UCAST, +1);
	
	
    /* Join the cluster to the MBlock */
	len = (pDrvCtrl ->RecvBufDesc[recvdescptr].recvstatus&(0x7ff));
	
	bcopy (  pDrvCtrl ->clPointBuf[recvdescptr], pNewCluster+2, len  );
/*
    netClBlkJoin ( pClBlk, pDrvCtrl ->clPointBuf[recvdescptr], at91endClDescTbl[0].clSize, NULL, 0, 0, 0);

*/
	netClBlkJoin ( pClBlk, pNewCluster, at91endClDescTbl[0].clSize, NULL, 0, 0, 0);
	
	netMblkClJoin (pMblk, pClBlk);

	tmp = 2;
	/*
	( ((pDrvCtrl ->RecvBufDesc)[recvdescptr].bufaddr&0xFFFFFFFC) 
			- (UINT32)(pDrvCtrl ->clPointBuf[recvdescptr]) );*/
	

	pMblk->mBlkHdr.mData += tmp;

    pMblk->mBlkHdr.mLen = len;
    pMblk->mBlkHdr.mFlags |= M_PKTHDR;
    pMblk->mBlkPktHdr.len = pMblk->mBlkHdr.mLen;

	(pDrvCtrl ->RecvBufDesc)[recvdescptr].bufaddr &= 0xFFFFFFFE;
	if( pDrvCtrl ->rxBufPtr == (RX_FD_NUM -1) )
	{
		pDrvCtrl ->rxBufPtr = 0;
	}
	else
		pDrvCtrl ->rxBufPtr ++;
	

    /* Call the upper layer's receive routine. */
    END_RCV_RTN_CALL( &pDrvCtrl->end, pMblk );

/*
	(pDrvCtrl ->clPointBuf)[recvdescptr] = pNewCluster;
	tmp = ( ( (UINT32)pNewCluster+3 )&0xFFFFFFFC );
	if( recvdescptr == (RX_FD_NUM -1) )
		tmp |= EMAC_RECV_DESC_WRAP;

	(pDrvCtrl ->RecvBufDesc)[recvdescptr].bufaddr = tmp;
*/
	ENDLOGMSG( ( "END: leave at91EndRecv *******.\n", 1,2,3,4,5,6) );
	return (OK);


cleanRXD:
	ENDLOGMSG( ( "END: packet lost because of get cl or m or clust buffer failure.\n", 1,2,3,4,5,6) );
	(pDrvCtrl ->RecvBufDesc)[recvdescptr].bufaddr &= 0xFFFFFFFE;
    return (OK);
}

/*******************************************************************************
*
* at91EndSend - the driver send routine
*
* This routine takes a M_BLK_ID sends off the data in the M_BLK_ID.
* The buffer must already have the addressing information properly installed
* in it.  This is done by a higher layer.  The last arguments are a free
* routine to be called when the device is done with the buffer and a pointer
* to the argument to pass to the free routine.  
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndSend
    (
    at91end_device *pDrvCtrl,	/* device ptr */
    M_BLK_ID pNBuff		/* data to send */
    )
{
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
    BOOL        freeNow = TRUE;
	UINT32		curtailptr, txdelta;
	UINT32 oldlevel;
	char *txbuf;

/*
	ENDLOGMSG ((" Enter  at91EndSend routine...\n", 1, 2, 3, 4, 5, 6));
*/


	oldlevel = intLock ();

	curtailptr = pDrvCtrl ->TxTailPtr;
	txdelta = TX_FD_NUM - curtailptr + (pDrvCtrl ->TxPtr);
	if( txdelta >= TX_FD_NUM )
		txdelta -= TX_FD_NUM;

	if( (txdelta < 3) && ( pDrvCtrl ->TXFifoStat != TXFIFOEMPTY ) )
	{
		intUnlock( oldlevel );
		return ERROR;
	}

	intUnlock( oldlevel );

    /*
     * Obtain exclusive access to transmitter.  This is necessary because
     * we might have more than one stack transmitting at once.
     */
	END_TX_SEM_TAKE ( &pDrvCtrl->end, WAIT_FOREVER );

	/*
	 *	fifo now have space for new packet
	 */
	txbuf = (pDrvCtrl ->pTxBufStart)[curtailptr].bufaddr;
	(pDrvCtrl ->pTxBufStart)[curtailptr].len = pNBuff->mBlkPktHdr.len;
	

    /* Set pointers in local structures to point to data. */
	netMblkToBufCopy( pNBuff, (void *)txbuf, NULL ) ;


    /* place a transmit request */
    oldlevel = intLock ();   /* now at91EndInt won't get confused */
	
	if( pDrvCtrl ->TxTailPtr == (TX_FD_NUM-1) )
		pDrvCtrl ->TxTailPtr = 0;
	else
		pDrvCtrl ->TxTailPtr ++;
	
	if( pDrvCtrl ->TxTailPtr == pDrvCtrl ->TxPtr )
		pDrvCtrl ->TXFifoStat = TXFIFOFULL;
	else
		pDrvCtrl ->TXFifoStat = TXFIFONORMAL;

	if( (pemac ->EMAC_TSR) & EMAC_TSR_BNQ ) 
	{
		/*
		 *	TxPtr ->next ready to send package
		 *	TxTailPtr ->Next to accept transmit package
		 */
		TranNextPackage( pDrvCtrl );
	}	
	
    intUnlock (oldlevel);   /* now at91EndInt won't get confused */
    

	END_TX_SEM_GIVE (&pDrvCtrl->end);
    
    /* Bump the statistic counter. */
    END_ERR_ADD (&pDrvCtrl->end, MIB2_OUT_UCAST, +1);

    /*
     * Cleanup.  The driver must either free the packet now or
     * set up a structure so it can be freed later after a transmit
     * interrupt occurs.
     */
    if (freeNow)
        netMblkClChainFree (pNBuff);

/*
	ENDLOGMSG ((" Leave  at91EndSend routine...\n", 1, 2, 3, 4, 5, 6));
*/
	

    return (OK);
}

/*******************************************************************************
*
* at91EndIoctl - the driver I/O control routine
*
* Process an ioctl request.
*
* RETURNS: A command specific response, usually OK or ERROR.
*/

LOCAL int at91EndIoctl
    (
    at91end_device *pDrvCtrl,	/* device receiving command */
    int cmd,			/* ioctl command code */
    caddr_t data		/* command argument */
    )
{

    int error = 0;
    long value;
/*
	ENDLOGMSG ((" Enter   at91EndIoctl routine...\n", 1, 2, 3, 4, 5, 6));
*/
	
    switch (cmd)
	{
	case EIOCSADDR:
		if (data == NULL)
			return (EINVAL);
		bcopy ((char *)data, (char *)END_HADDR(&pDrvCtrl->end),
			END_HADDR_LEN(&pDrvCtrl->end));
		break;
		
	case EIOCGADDR:
		if (data == NULL)
			return (EINVAL);
		bcopy ((char *)END_HADDR(&pDrvCtrl->end), (char *)data,
			END_HADDR_LEN(&pDrvCtrl->end));
		break;
		
	case EIOCSFLAGS:
		value = (long)data;
		if (value < 0)
		{
			value = ~value;
			END_FLAGS_CLR (&pDrvCtrl->end, value);
		}
		else
		{
			END_FLAGS_SET (&pDrvCtrl->end, value);
		}
		at91EndConfig (pDrvCtrl);
		break;
		
	case EIOCGFLAGS:
		*(int *)data = END_FLAGS_GET(&pDrvCtrl->end);
		break;
		
	case EIOCPOLLSTART:
		at91EndPollStart (pDrvCtrl);
		break;
		
	case EIOCPOLLSTOP:
		at91EndPollStop (pDrvCtrl);
		break;
		
    case EIOCGMIB2:
		if (data == NULL)
            return (EINVAL);
		bcopy((char *)&pDrvCtrl->end.mib2Tbl, (char *)data,
            sizeof(pDrvCtrl->end.mib2Tbl));
		break;
		
    case EIOCGFBUF:
		if (data == NULL)
			return (EINVAL);
		*(int *)data = 0;	/**END_MIN_FBUF;**/
		break;
		
    default:
		error = EINVAL;
	}
/*
	ENDLOGMSG ((" Leave   at91EndIoctl routine...\n", 1, 2, 3, 4, 5, 6));
*/
    return (error);
}

/******************************************************************************
*
* at91EndConfig - reconfigure the interface under us.
*
* Reconfigure the interface setting promiscuous mode
*
* RETURNS: N/A.
*/

LOCAL void at91EndConfig
    (
    at91end_device *pDrvCtrl	/* device to be re-configured */
    )
{
	at91EmacReset(pDrvCtrl);	/* reset the chip */

    /* Set up address filter for multicasting. */
    if ( END_MULTI_LST_CNT( &pDrvCtrl->end ) > 0 )
		at91EndAddrFilterSet ( pDrvCtrl );

	at91EndFdFree(pDrvCtrl);	/* Free the FDs */
	at91EndFdInitialize(pDrvCtrl);	/* Reinitialize FDs */
	
	at91EndMacInitialize( pDrvCtrl );	/* Initialize MAC */

	/* Was started before in interrupt mode? */
	if ( (END_FLAGS_GET(&pDrvCtrl->end) & IFF_RUNNING) &&
			((pDrvCtrl->flags & LS_POLLING) == 0) )
			at91EndStart(pDrvCtrl);	/* Start again */

    return;
    }

/*******************************************************************************
*
* at91EndPollStart - start polled mode operations
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndPollStart
    (
    at91end_device* pDrvCtrl	/* device to be polled */
    )
{

    int         oldLevel;
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	
    oldLevel = intLock ();          /* disable ints during update */

	intDisable (pDrvCtrl->level);

    pDrvCtrl->flags |= LS_POLLING;

    intUnlock (oldLevel);   /* now at91EndInt won't get confused */

    ENDLOGMSG (( "POLLing STARTED\n", 1, 2, 3, 4, 5, 6 ));

    at91EndConfig (pDrvCtrl);	/* reconfigure device */

	/*
	 *	enable receive
	 */
	pemac ->EMAC_CTL = EMAC_CTL_RE | EMAC_CTL_TE;

	/* Set the flags to indicate that the device is up */
	END_FLAGS_SET (&pDrvCtrl->end, IFF_UP | IFF_RUNNING);

    return (OK);
    }

/*******************************************************************************
*
* at91EndPollStop - stop polled mode operations
*
* This function terminates polled mode operation.  The device returns to
* interrupt mode.
*
* The device interrupts are enabled, the current mode flag is switched
* to indicate interrupt mode and the device is then reconfigured for
* interrupt operation.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndPollStop
    (
    at91end_device* pDrvCtrl	/* device to be changed */
    )
{
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	int         oldLevel;

	/*
	 *	stop all
	 */
	pemac ->EMAC_CTL = 0;

    oldLevel = intLock ();	/* disable ints during register updates */

    pDrvCtrl->flags &= ~LS_POLLING;

    intUnlock (oldLevel);

	/*
	 * Restart in interrupt mode.  Calling at91EndConfig without clearing
	 * IFF_RUNNING flag will result in calling at91EndStart automatically
	 */
    at91EndConfig (pDrvCtrl);

    ENDLOGMSG (("END POLLing STOPPED\n", 1, 2, 3, 4, 5, 6));
	return (OK);
}

/*******************************************************************************
*
* at91EndPollRcv - routine to receive a packet in polled mode.
*
* This routine is called by a user to try and get a packet from the
* device.
*
* RETURNS: OK upon success.  EAGAIN is returned when no packet is available.
*/

LOCAL STATUS at91EndPollRcv
    (
    at91end_device *pDrvCtrl,	/* device to be polled */
    M_BLK_ID pMblk		/* ptr to buffer */
	)
{

	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
    int len;
	UINT32 rxptr;
	STATUS retVal = EAGAIN;

	ENDLOGMSG (( "entering at91EndPollRcv\n", 1, 2, 3, 4, 5, 6 ));
	
	rxptr = pDrvCtrl ->rxBufPtr;
	
	if( ( (pDrvCtrl ->RecvBufDesc[rxptr].bufaddr) & EMAC_RECV_DESC_HAVE_DATA ) == 0 )
		return EAGAIN;

	/* Upper layer must provide a valid buffer. */
	len =  pDrvCtrl ->RecvBufDesc[rxptr].recvstatus & (0x7FF) ;

    if ((pMblk->mBlkHdr.mLen < len) || (!(pMblk->mBlkHdr.mFlags & M_EXT)))
	{
		ENDLOGMSG( ( "END: poll receive buffer too small to fit received buffer\n", 1,2,3,4,5,6 ) );
		goto pollRcvExit;
	}

    END_ERR_ADD (&pDrvCtrl->end, MIB2_IN_UCAST, +1);

	bcopy ( (char*)(pDrvCtrl ->RecvBufDesc[rxptr].bufaddr&0xFFFFFFFC), pMblk->m_data, len );
    pMblk->m_len = len;
	/*pMblk->mBlkHdr.mData += OFFSET;*/
    pMblk->mBlkHdr.mFlags |= M_PKTHDR;
    pMblk->mBlkPktHdr.len = len;

	retVal = OK;

	ENDLOGMSG ((" proces at91EndPollRcv OK\n", 1, 2, 3, 4, 5, 6));

pollRcvExit:

	pDrvCtrl ->RecvBufDesc[rxptr].bufaddr &= 0xFFFFFFFE;
	if( rxptr == (RX_FD_NUM-1) )
		pDrvCtrl ->rxBufPtr = 0;
	else
		pDrvCtrl ->rxBufPtr ++;

	return retVal;
	
}

/*******************************************************************************
*
* at91EndPollSend - routine to send a packet in polled mode.
*
* This routine is called by a user to try and send a packet on the
* device.
*
* RETURNS: OK upon success.  EAGAIN if device is busy.
*/

LOCAL STATUS at91EndPollSend
    (
    at91end_device* pDrvCtrl,	/* device to be polled */
    M_BLK_ID pMblk		/* packet to send */
    )
{
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	UINT32 curTxptr = pDrvCtrl ->TxPtr;

    int         len;
	ENDLOGMSG ((" enter at91EndPollSend\n", 1, 2, 3, 4, 5, 6 ));

	if( !(pemac ->EMAC_TSR&EMAC_TSR_TXIDLE ) )
	{
		return EAGAIN;
	}

	len = pMblk->mBlkPktHdr.len;
    /* Set pointers in local structures to point to data. */
	netMblkToBufCopy(pMblk, (void *)pDrvCtrl ->pTxBufStart[curTxptr].bufaddr, NULL) ;
	pDrvCtrl ->pTxBufStart[curTxptr].len = len;

	/*
	 *	start transmit
	 */
	pemac ->EMAC_TAR = (UINT32)pDrvCtrl ->pTxBufStart[curTxptr].bufaddr;
	pemac ->EMAC_TCR = len&(0x7FF);


    /* Bump the statistic counter. */

    END_ERR_ADD (&pDrvCtrl->end, MIB2_OUT_UCAST, +1);


    /*
     * Cleanup.  The driver must either free the packet now or
     * set up a structure so it can be freed later after a transmit
     * interrupt occurs.
     */
	netMblkClChainFree (pMblk);

	ENDLOGMSG (("leaving at91EndPollSend\n", 1, 2, 3, 4, 5, 6));

    return (OK);
}

/******************************************************************************
*
* at91EndAddrFilterSet - set the address filter for multicast addresses
*
* This routine goes through all of the multicast addresses on the list
* of addresses (added with the endAddrAdd() routine) and sets the
* device's filter correctly.
*
* RETURNS: N/A.
*/

LOCAL void at91EndAddrFilterSet
    (
    at91end_device *pDrvCtrl	/* device to be updated */
	)
{
    ETHER_MULTI* pCurr;
    UINT32 count;

	ENDLOGMSG ((" Enter   at91EndAddrFilterSet routine...\n", 1, 2, 3, 4, 5, 6));

	for ( count = 0; count < (AT91EMAC_MAX_MULTI+1); count++ )
	{
		pDrvCtrl->addrListLow[count] =	/* Zero the addresses */
			pDrvCtrl->addrListHigh[count] = 0;
	}

	pDrvCtrl->mcastAddrCount = 0;		/* Init the multi count */
	
    pCurr = END_MULTI_LST_FIRST ( &pDrvCtrl->end );

	/*
	 * Now copy the addresses from ether_multi_list to our own array.  In our
	 * array, the first 6 bytes are for our own MAC address.  This array is
	 * an exact replica of the internal CAM registers of Ethernet controller.
	 * The CAM registers will be updated in at91EndMacInitialize() function
	 */
    while ( pCurr != NULL )
	{
		if (pDrvCtrl->mcastAddrCount > AT91EMAC_MAX_MULTI )
			break;
		FormatETHSpecialAddrWord( &( pDrvCtrl ->addrListHigh[ pDrvCtrl ->mcastAddrCount + 1 ] ), 
			&(pDrvCtrl ->addrListLow[ pDrvCtrl ->mcastAddrCount + 1 ]), (BYTE*)(pCurr->addr) );
		

		pDrvCtrl ->mcastAddrCount++;		/* Bump the multicast address count */
	
		pCurr = END_MULTI_LST_NEXT(pCurr);	/* Get the next address in the list */
	}
	ENDLOGMSG ((" Leave   at91EndAddrFilterSet routine...\n", 1, 2, 3, 4, 5, 6));
	
}

/*****************************************************************************
*
* at91EndMCastAdd - add a multicast address for the device
*
* This routine adds a multicast address to whatever the driver
* is already listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndMCastAdd
    (
    at91end_device *pDrvCtrl,	/* device pointer */
    char* pAddress		/* new address to add */
    )
{
    int error;

    if ( (error = etherMultiAdd (&pDrvCtrl->end.multiList,
		pAddress)) == ENETRESET )
	    at91EndConfig (pDrvCtrl);

    return (OK);
}

/*****************************************************************************
*
* at91EndMCastDel - delete a multicast address for the device
*
* This routine removes a multicast address from whatever the driver
* is listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndMCastDel
    (
    at91end_device* pDrvCtrl,	/* device pointer */
    char* pAddress		/* address to be deleted */
    )
{
    int error;

    if ((error = etherMultiDel (&pDrvCtrl->end.multiList,
	     (char *)pAddress)) == ENETRESET)
	    at91EndConfig (pDrvCtrl);
    return (OK);
}

/*****************************************************************************
*
* at91EndMCastGet - get the multicast address list for the device
*
* This routine gets the multicast list of whatever the driver
* is already listening for.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndMCastGet
    (
    at91end_device* pDrvCtrl,	/* device pointer */
    MULTI_TABLE* pTable		/* address table to be filled in */
    )
{
    int error;

    error = etherMultiGet (&pDrvCtrl->end.multiList, pTable);

    return (error);
}

/*******************************************************************************
*
* at91EndStop - stop the device
*
* This function calls BSP functions to disconnect interrupts and stop
* the device from operating in interrupt mode.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS at91EndStop
    (
    at91end_device *pDrvCtrl	/* device to be stopped */
    )
{
	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	ENDLOGMSG ((" Enter   at91EndStop routine...\n", 1, 2, 3, 4, 5, 6));
	
	/*
	 *	disable rx and tx, and int
	 */
	pemac ->EMAC_CTL = EMAC_CTL_CSR;
	pemac ->EMAC_IDR = 0xFFFFFFFF;

	intDisable ( pDrvCtrl->level );
	END_FLAGS_CLR (&pDrvCtrl->end, IFF_UP | IFF_RUNNING);
	ENDLOGMSG ((" Leave   at91EndStop routine...\n", 1, 2, 3, 4, 5, 6));
	
    return (OK);
}

/******************************************************************************
*
* at91EndUnload - unload a driver from the system
*
* This function first brings down the device, and then frees any
* stuff that was allocated by the driver in the load function.
*
* RETURNS: OK or ERROR.
*/

LOCAL void FreeEndMemory(  at91end_device* pDrvCtrl )
{
	if ( pDrvCtrl->end.pNetPool )
	{
		netPoolDelete (pDrvCtrl->end.pNetPool);
		free (pDrvCtrl->end.pNetPool);
		pDrvCtrl->end.pNetPool = (NET_POOL_ID)0;
	}
	
	if (at91endClDescTbl[0].memArea)
	{
		cacheDmaFree (at91endClDescTbl[0].memArea);
		at91endClDescTbl[0].memArea = (char *)0;
	}
	
	if( pDrvCtrl ->TranFifoBuf )
	{
		cacheDmaFree( pDrvCtrl ->TranFifoBuf );
		pDrvCtrl ->RecvBufDesc = NULL;
		pDrvCtrl ->TranFifoBuf = NULL;
		pDrvCtrl ->RecvFifoBuf = NULL;
		bzero( (void*)&(pDrvCtrl->pTxBufStart[0]), sizeof( AT91_END_TRAN_FD	)*TX_FD_NUM );
	}
	
	if (at91endMclConfig.memArea)
	{
		free (at91endMclConfig.memArea);
		at91endMclConfig.memArea = (char *)0;
	}
	
	if (pDrvCtrl)
	{
		free (pDrvCtrl);
		(at91end_device*)pDrvCtrl = 0;
	}

}

LOCAL STATUS at91EndUnload
    (
    at91end_device* pDrvCtrl	/* device to be unloaded */
    )
{
    END_OBJECT_UNLOAD (&pDrvCtrl->end);   /*this call will indicate */
										  /* protocol that the device will be unloaded */

    /* TODO - Free any shared DMA memory */
	at91EndStop(pDrvCtrl);   	/*before unloading the device will stop functioning*/
	at91EmacReset(pDrvCtrl);		/*then it will reset the registers in order to clear any pending interrupts */
	at91EndFdFree(pDrvCtrl);
	FreeEndMemory( pDrvCtrl );

    return (OK);
}

/******************************************************************************
* at91EmacReset - Reset the device
* This function resets the driver after initializing from at91EndLoad
*/

LOCAL void	at91EmacReset
    (
    at91end_device* pDrvCtrl	/* device to be reset */
    )
{
	UINT32 dummy, i;
	UINT32 tmp = 0xFFFFFFFF;
	EMAC_S *pemac = (EMAC_S*)EMAC_BASE_ADDR;
	if(pDrvCtrl->unit != 0)
		return;
	/*
	 *	disable interrupt first
	 */
	pemac ->EMAC_IDR = 0xFFFFFFFF;
	
	/*
	 *	disable recive and transmit 
	 */
	pemac ->EMAC_CTL = EMAC_CTL_CSR;
	
	/*
	 *	config EMAC state, MAX HCLK = (60-100)/16 < 8M (for LXT971)
	 */
	pemac ->EMAC_CFG = EMAC_CFG_CLK_HCLK_16 | EMAC_CFG_SPD | EMAC_CFG_RMII /*| EMAC_CFG_MTI | EMAC_CFG_UNI*/;
	
	/*
	 *	clear transmit and receive status bit
	 */
	pemac ->EMAC_TSR = tmp;
	pemac ->EMAC_RSR = tmp;
	
	/*
	 *	clear interrupt status REG
	 */
	dummy = pemac ->EMAC_ISR;
	
	/*
	 *	set all statistic regoster by read it
	 */
	for( i = 0; i< sizeof(ETH_STAT_S)/sizeof(AT91_REG); i++ )
	{
		dummy = ( (AT91_REG*)( &(pemac ->ESTAT ) ) )[i];
	}
	
	/*
	 *	after reset, we need to set address for mac
	 */

	/*
	 *	
	 */


	return;
}

/******************************************************************************
* at91EndPhyRead - Read PHY device
* This function is used to read a byte from the PHY device
*/
LOCAL UINT32 at91EndPhyRead
	(
	UINT32 phyRegAddr, 	/* Address of PHY register to be read */
	UINT32 phyAddr		/* Address of the PHY chip (usually 0 for single PHY) */
	)
{
	EMAC_S* regs = (EMAC_S*)EMAC_BASE_ADDR;
	UINT32 dummy;

	regs->EMAC_MAN = EMAC_PHY_HIGH | EMAC_PHY_CODE | EMAC_PHY_RW_R
		| (phyRegAddr << 18);
	
	/* Wait until IDLE bit in Network Status register is cleared */
	/* TODO: Enforce some maximum loop-count?*/
	dummy = 0;
	while ( !(regs->EMAC_SR & EMAC_SR_IDLE)&& (dummy++ < 10000) ) ;
		
	return ( regs->EMAC_MAN & 0x0000ffff );
}

/******************************************************************************
* at91EndPhyWrite	- Wrire into PHY device
* This function is used to write a byte to the PHY device
*/
LOCAL void at91EndPhyWrite
	(
	UINT32 phyRegAddr, 	/* Address of PHY register to be written */
	UINT32 phyAddr,		/* Address of the PHY chip (usually 0 for single PHY) */
	UINT32 phyData		/* Data to be written */
	)
{
	EMAC_S* regs = (EMAC_S*)EMAC_BASE_ADDR;
	UINT32 dummy;

	regs->EMAC_MAN = (EMAC_PHY_HIGH | EMAC_PHY_CODE | EMAC_PHY_RW_R
		| (phyRegAddr << 18)) + (phyData & 0xffff);
	
	/* Wait until IDLE bit in Network Status register is cleared */
	/* TODO: Enforce some maximum loop-count?*/
	dummy = 0;
	while (!(regs->EMAC_SR & EMAC_SR_IDLE)&(dummy++ < 10000) ) ;
}


/******************************************************************************
* at91EndMacIntialize - Initialize MAC/BDMA registers
* Initialize the MAC and BDMA registers to make the Ethernet interface functional
*/
LOCAL void at91EndMacInitialize
	(
	at91end_device *pDevice	/* Device that has to be initialized */
	)
{

	EMAC_S* pemac = (EMAC_S*)EMAC_BASE_ADDR;
	
	/*
	 *	set the first address as the IF mac address
	 */
	FormatETHSpecialAddrWord( &(pDevice ->addrListHigh[0]), 
		&(pDevice ->addrListLow[0]), (BYTE*)pDevice->enetAddr );

	/*
	 *	write the special address to chip
	 */
	pemac ->EMAC_SA1L = pDevice ->addrListLow[0];
	pemac ->EMAC_SA1H = pDevice ->addrListHigh[0];

	if( pDevice ->mcastAddrCount > 0 )
	{
		pemac ->EMAC_SA2L = pDevice ->addrListLow[1];
		pemac ->EMAC_SA2H = pDevice ->addrListHigh[1];
	}
	else
	{
		pemac ->EMAC_SA2L = 0;
		pemac ->EMAC_SA2H = 0;
	}

	if( pDevice ->mcastAddrCount > 1 )
	{
		pemac ->EMAC_SA3L = pDevice ->addrListLow[2];
		pemac ->EMAC_SA3H = pDevice ->addrListHigh[2];
	}
	else
	{
		pemac ->EMAC_SA3L = 0;
		pemac ->EMAC_SA3H = 0;
	}
	
	if( pDevice ->mcastAddrCount > 2 )
	{
		pemac ->EMAC_SA4L = pDevice ->addrListLow[3];
		pemac ->EMAC_SA4H = pDevice ->addrListHigh[3];
	}
	else
	{
		pemac ->EMAC_SA4L = 0;
		pemac ->EMAC_SA4H = 0;
	}

	/*
	 *	what use for hash reg?, write all one first
	 */
	pemac ->EMAC_HSH = 0xFFFFFFFF;
	pemac ->EMAC_HSL = 0xFFFFFFFF;

	/*
	 *	setup receive buffer address
	 */
	pemac ->EMAC_RBQP = (AT91_REG)(pDevice ->RecvBufDesc);
	
	/*
	 *	write configuration register again, do not allow mulitcast and unicast 
	 */
	pemac ->EMAC_CFG  = EMAC_CFG_CLK_HCLK_16 | EMAC_CFG_SPD | EMAC_CFG_RMII /*| EMAC_CFG_MTI | EMAC_CFG_UNI*/;

	
	return;
	
}


/**********************************************************************************
*    at91EndFdInitialize - Initialize TX and RX FD lists
*    Make a circular list of Rx and TX frame descriptors and buffers.
*    Two global variables gpReceiveFrameDescStart and gpTransmitFrameDescStart 
*    stores the pointer to the start of the list.  BDMA TX/RX PTR registers are 
*    also initialized with the start of the appropriate list.
*/

LOCAL STATUS at91EndFdInitialize
    (
    at91end_device* pDrvCtrl	/* device to be initialized */
    )
{

    int count;

	char *pNewCluster;

	if( pDrvCtrl ->RecvFifoBuf == NULL )
	{
		ENDLOGMSG( ( "END: at91EndFdInitialize find RX fifo have no buffer, something gong wrong ! ", 1,2,3,4,5,6 ));
		at91EndFdFree( pDrvCtrl );
		return (ERROR);

	}

	for( count = 0; count < RX_FD_NUM; count++ )
	{
	    
		pNewCluster = pDrvCtrl ->RecvFifoBuf + count*(pDrvCtrl ->TranPkgSize);
		/*	netClusterGet ( pDrvCtrl->end.pNetPool, pDrvCtrl->pClPoolId );*/
		
	
/*
		if ( pNewCluster == NULL )
	    {
			ENDLOGMSG(( "END:Get clust failure in routine at91EndFdInitialize\n", 1,2,3,4,5, 6 ));
			at91EndFdFree( pDrvCtrl );
			return ERROR;
	    }
*/

		pDrvCtrl ->clPointBuf[ count ] = pNewCluster;

		if( ((UINT32)pNewCluster)&0x3 != 0 )
		{
			ENDLOGMSG(( "END:receive buffer address not at wordalign in routine at91EndFdInitialize. point: %x\n", (UINT32)pNewCluster ,2,3,4,5, 6 ))
		}

		/*
		 *	just make the point align to word
		 */
		(pDrvCtrl ->RecvBufDesc)[count].bufaddr = ( ( ((UINT32)pNewCluster) )&(0xFFFFFFFC) );
		(pDrvCtrl ->RecvBufDesc)[count].recvstatus = 0;

		ENDLOGMSG(("the 0x%x addr=0x%x\n",count,(pDrvCtrl ->RecvBufDesc)[count].bufaddr,3,4,5,6))

	}/* end of for loop*/

	(pDrvCtrl ->RecvBufDesc)[RX_FD_NUM - 1].bufaddr |= EMAC_RECV_DESC_WRAP;
	pDrvCtrl ->rxBufPtr = 0;

	/*
	 *	initialize tx desc query
	 */
	if( (pDrvCtrl ->TranFifoBuf) == NULL )
	{
		ENDLOGMSG( ( "END: at91EndFdInitialize find TX fifo have no buffer, something gong wrong ! ", 1,2,3,4,5,6 ));
		at91EndFdFree( pDrvCtrl );
		return (ERROR);
	}

	for( count = 0; count < TX_FD_NUM; count ++ )
	{
		(pDrvCtrl ->pTxBufStart)[ count ].bufaddr = 
			pDrvCtrl ->TranFifoBuf + count*(pDrvCtrl ->TranPkgSize);
		(pDrvCtrl ->pTxBufStart)[ count ].owner = 0;	/*init owner to software */
	}
	pDrvCtrl ->TxTailPtr = pDrvCtrl ->TxPtr = 0;
	pDrvCtrl ->TXFifoStat = TXFIFOEMPTY;

	pDrvCtrl->fdInitialized = TRUE;
	return OK;
}

/******************************************************************************
*	at91EndFdFree - Free the allocated TX and RX FD lists and buffers
*	This function frees all the allocated TX and RX FDs and the associated
*	buffers
*/

LOCAL void at91EndFdFree
    (
    at91end_device* pDrvCtrl	/* device to be freed */
    )
{
	UINT32 count;
	char* tmpcl;

/*
	if (pDrvCtrl->fdInitialized == FALSE)
		return;
*/
/*
	if( pDrvCtrl ->RecvBufDesc )
	{
		for ( count = 0; count < RX_FD_NUM; count++ )
		{
			tmpcl = pDrvCtrl ->clPointBuf[ count ];
			if( tmpcl )
			{
				netClFree (pDrvCtrl->end.pNetPool, tmpcl );
			}
			(pDrvCtrl ->RecvBufDesc)[count].bufaddr = 0;
			pDrvCtrl ->clPointBuf[ count ] = NULL;
		}
	}
*/
	pDrvCtrl->fdInitialized = FALSE;
}


#endif 	/* INCLUDE_AT91EMAC_END */
