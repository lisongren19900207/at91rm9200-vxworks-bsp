/* at91DBGU.c - AT91RM9200 DBGU driver */

/*
modification history
--------------------

*/

/*
DESCRIPTION

INCLUDES:
at91DBGU.h sioLib.h
*/

#include "sioLib.h"
#include "intLib.h"
#include "errno.h"
#include "ioLib.h"


#define AT91DBGU_BAUD_MIN	1200
#define AT91DBGU_BAUD_MAX	460800
							
/* for backward compatibility */

#ifndef	SIO_HUP
#   define SIO_OPEN	0x100A	/* open channel, raise DTR, RTS */
#   define SIO_HUP	0x100B	/* hang-up, lower DTR, RTS */
#endif

/* forward static declarations */
void AT91_PDC_Close ( PDC_S * pPDC );       

LOCAL int	AT91DBGUTxStartup (SIO_CHAN * pSioChan);
LOCAL int	AT91DBGUCallbackInstall (SIO_CHAN *pSioChan, int callbackType,
				    STATUS (*callback)(), void *callbackArg);
LOCAL int	AT91DBGUPollOutput (SIO_CHAN *pSioChan, char	outChar);
LOCAL int	AT91DBGUPollInput (SIO_CHAN *pSioChan, char *thisChar);
LOCAL int	AT91DBGUIoctl (SIO_CHAN *pSioChan, int request, void *arg );

LOCAL STATUS dummyCallback (void);

/* local variables */

LOCAL	SIO_DRV_FUNCS AT91DBGUDrvFuncs =
{
    AT91DBGUIoctl,
    AT91DBGUTxStartup,
    AT91DBGUCallbackInstall,
    AT91DBGUPollInput,
    AT91DBGUPollOutput,
};



/******************************************************************************
*
* AT91DBGUDevInit - initialize a AT91_DBGU
*
* This routine initializes the driver
* function pointers and then resets the chip in a quiescent state.
* The BSP must have already initialized all the device addresses and the
* baudFreq fields in the AT91_DBGU structure before passing it to
* this routine.
*
* RETURNS: N/A
*/

void AT91DBGUDevInit
(
    AT91_DBGU_CHAN * pChan
)
{
    UINT32 dummy;
	/* initialize each channel's driver function pointers */
    int oldlevel;
	
    pChan->sio.pDrvFuncs	= &AT91DBGUDrvFuncs;

    /* install dummy driver callbacks */

    pChan->getTxChar    = dummyCallback;
    pChan->putRcvChar	= dummyCallback;
    
    /* reset the chip */
	 oldlevel = intLock ();
	
    pChan->regs->DBGU_CR = DBGU_CR_RESET_RECV | DBGU_CR_RESET_TRAN | DBGU_CR_RX_DIS | DBGU_CR_TX_DIS;
	pChan->regs->DBGU_MR = DBGU_MR_PARITY_NONE | DBGU_MR_TEST_NORMAL_MODE;
	/*
	 *	disable all interrupt source first
	 */
	pChan->regs->DBGU_IDR = 0xFFFFFFFF;
	dummy = pChan ->regs->DBGU_RHR;
    intUnlock ( oldlevel );
	pChan->intrmode = FALSE;
	
	/*
	 *	Init PDC, disable all pdc function
	 */
	AT91_PDC_Close( &(pChan->regs->DBGU_PDC) );
	/*
	 *	make a dummy read emtpy the receive buf
	 */
	
    /* setting polled mode is one way to make the device quiet */
	AT91DBGUIoctl ( (SIO_CHAN *)pChan, SIO_BAUD_SET, (void *)pChan->baudRate );
    AT91DBGUIoctl ( (SIO_CHAN *)pChan, SIO_MODE_SET,(void *)SIO_MODE_POLL );

	pChan->regs->DBGU_CR = DBGU_CR_RX_ENA | DBGU_CR_TX_DIS | DBGU_CR_RESET_STATUS;
}

/******************************************************************************
*
* AT91DBGUDevInit2 - initialize a AT91_DBGU, part 2
*
* This routine is called by the BSP after interrupts have been connected.
* The driver can now operate in interrupt mode.  Before this routine is
* called only polled mode operations should be allowed.
*
* RETURNS: N/A
* ARGSUSED
*/

void AT91DBGUDevInit2
    (
    AT91_DBGU_CHAN * pChan		/* device to initialize */
    )
{
	pChan ->errcount = 0;
	pChan->regs->DBGU_CR = DBGU_CR_RX_ENA | DBGU_CR_TX_DIS | DBGU_CR_RESET_STATUS;
	/*
	 *	enable DBGU interrupt now.
	 */
    pChan->regs->DBGU_IER = DBGU_IE_DR_TXRDY | DBGU_IE_DR_RXRDY | DBGU_IE_DR_FRAME 
					| DBGU_IE_DR_PARE| DBGU_IE_DR_OVRE;

	pChan->intrmode = TRUE;
}

/******************************************************************************
*
* AT91DBGUInt - handle a channel's receive-character interrupt
*
* RETURNS: N/A
*/ 

void AT91DBGUInt
    (
    AT91_DBGU_CHAN *	pChan		/* channel generating the interrupt */
    )
{
    
	char outchar;
	UINT32	    status;

	DBGU_S* udev = pChan ->regs;
	
	status = udev->DBGU_CSR;
	if( status&( DBGU_CSR_FRAME | DBGU_CSR_OVRE| DBGU_CSR_PARE ) )
	{
		pChan->errcount ++;
		udev->DBGU_CR = DBGU_CR_RESET_STATUS;
	}

	if( status&(DBGU_CSR_RXRDY ) )
	{
		(*pChan->putRcvChar) ( pChan->putRcvArg, udev ->DBGU_RHR );
	}

	if( status&( DBGU_CSR_TXRDY ) )
	{
		if( (*pChan->getTxChar) (pChan->getTxArg, &outchar) != ERROR )
		{
			udev ->DBGU_THR = (AT91_REG)(outchar);
		}
		else
		{
			udev ->DBGU_CR = DBGU_CR_TX_DIS;
		}
	}
}


/******************************************************************************
*
* AT91DBGUTxStartup - start the interrupt transmitter
*
* RETURNS: OK on success, ENOSYS if the device is polled-only, or
* EIO on hardware error.
*/

LOCAL int AT91DBGUTxStartup
    (
    SIO_CHAN * pSioChan                 /* channel to start */
    )
{
    AT91_DBGU_CHAN * pChan = (AT91_DBGU_CHAN *)pSioChan;
	
	if( pChan ->intrmode == TRUE )
	{
		pChan ->regs->DBGU_CR = DBGU_CR_TX_ENA;
		return OK;
	}
	else 
		return ENOSYS;
}

/******************************************************************************
*
* AT91DBGUCallbackInstall - install ISR callbacks to get/put chars
*
* This driver allows interrupt callbacks for transmitting characters
* and receiving characters. In general, drivers may support other
* types of callbacks too.
*
* RETURNS: OK on success, or ENOSYS for an unsupported callback type.
*/ 

LOCAL int AT91DBGUCallbackInstall
    (
    SIO_CHAN *	pSioChan,               /* channel */
    int		callbackType,           /* type of callback */
    STATUS	(*callback)(),          /* callback */
    void *      callbackArg             /* parameter to callback */
    )
{
    AT91_DBGU_CHAN * pChan = (AT91_DBGU_CHAN *)pSioChan;
    switch (callbackType)
	{
	case SIO_CALLBACK_GET_TX_CHAR:
	    pChan->getTxChar	= callback;
	    pChan->getTxArg	= callbackArg;
	    return (OK);

	case SIO_CALLBACK_PUT_RCV_CHAR:
	    pChan->putRcvChar	= callback;
	    pChan->putRcvArg	= callbackArg;
	    return (OK);

	default:
	    return (ENOSYS);
	}
}

/*******************************************************************************
*
* AT91DBGUPollOutput - output a character in polled mode
*
* RETURNS: OK if a character arrived, EIO on device error, EAGAIN
* if the output buffer if full. ENOSYS if the device is
* interrupt-only.
*/

LOCAL int AT91DBGUPollOutput
    (
    SIO_CHAN *	pSioChan,
    char	outChar
    )
{
    AT91_DBGU_CHAN * pChan = (AT91_DBGU_CHAN *)pSioChan;
    DBGU_S* udev;
	UINT32	status;

	udev = pChan ->regs;
    /* is the transmitter ready to accept a character? */
	status = udev ->DBGU_CSR;

	if( status&( DBGU_CSR_FRAME | DBGU_CSR_OVRE| DBGU_CSR_PARE ) )
	{
		pChan->errcount ++;
		udev->DBGU_CR = DBGU_CR_RESET_STATUS;
	}

	if( status&(DBGU_CSR_TXRDY) )
	{
		udev ->DBGU_THR = (AT91_REG)(outChar);
		return (OK);
	}
	else
		return (EAGAIN);
}

/******************************************************************************
*
* AT91DBGUPollInput - poll the device for input
*
* RETURNS: OK if a character arrived, EIO on device error, EAGAIN
* if the input buffer if empty, ENOSYS if the device is
* interrupt-only.
*/

LOCAL int AT91DBGUPollInput
    (
    SIO_CHAN *	pSioChan,
    char *	thisChar
    )
{
    AT91_DBGU_CHAN * pChan = (AT91_DBGU_CHAN *)pSioChan;
    DBGU_S* udev;
	UINT32	status;
	
	udev = pChan ->regs;
	status = udev ->DBGU_CSR;
	
	if( status&( DBGU_CSR_FRAME | DBGU_CSR_OVRE| DBGU_CSR_PARE ) )
	{
		pChan->errcount ++;
		udev->DBGU_CR = DBGU_CR_RESET_STATUS;
	}
	
	if( status&(DBGU_CSR_RXRDY) )
	{
		status = udev ->DBGU_RHR;
		(*thisChar) = (BYTE)status;
	    return (OK);
    }
	else
		return (EAGAIN);
}
/******************************************************************************
*
* AT91DBGUModeSet - toggle between interrupt and polled mode
*
* RETURNS: OK on success, EIO on unsupported mode.
*/


/*
 *	because DBUG use the system interrupt line, we can not use intDisable/intEnable
 *	as normal UART 
 */
LOCAL int AT91DBGUModeSet
    (
    AT91_DBGU_CHAN * pChan,		/* channel */
    int	newMode        	/* new mode */
    )
{
    UINT32	temp;
    if ( (newMode != SIO_MODE_POLL) && (newMode != SIO_MODE_INT) )	
		return (EIO);


    /* Don't enter interrupt mode unless it is allowed. */
    if ( (newMode == SIO_MODE_INT) && ( pChan ->intrmode ) )
		return (EIO);

    /* set the new mode */
	if( newMode == SIO_MODE_INT )
		pChan->intrmode = TRUE;
	else
		pChan->intrmode = FALSE;


    if ( pChan->intrmode )
	{
		/*
		 *	TX int still not enabled
		 */
		pChan->regs->DBGU_CR = DBGU_CR_RX_ENA | DBGU_CR_TX_DIS | DBGU_CR_RESET_STATUS;
		pChan->regs->DBGU_IER = DBGU_IE_DR_TXRDY | DBGU_IE_DR_RXRDY | DBGU_IE_DR_FRAME 
			| DBGU_IE_DR_PARE| DBGU_IE_DR_OVRE;
/*
		intEable( pChan ->level );
*/
	}
	else
	{
		/*
		 *	disable all interrupts
		 */
		pChan->regs->DBGU_IDR = 0xFFFFFFFF;
		pChan->regs->DBGU_CR = DBGU_CR_RX_ENA | DBGU_CR_TX_ENA | DBGU_CR_RESET_STATUS;
		
/*
		intDisable( pChan ->level );
*/
	}
    return (OK);
}


/*******************************************************************************
*
* AT91DBGUHup - hang up the modem control lines 
*
* Resets the RTS and DTR signals.
*
* RETURNS: OK
*/

LOCAL STATUS AT91DBGUHup
    (
    AT91_DBGU_CHAN * pChan 	/* pointer to channel */
    )
{
/*
    FAST int     oldlevel;	/ * current interrupt level mask * /

    oldlevel = intLock ();

    / * set RTS and DTR low * /

/ *    SNDS_REG_WRITE(pChan, SNDS_USTAT, USTAT_DTR_LOW);	    	* /
    intUnlock (oldlevel);
*/
    return (OK);
}    

/*******************************************************************************
*
* AT91DBGUOpen - Set the modem control lines 
*
* Set the modem control lines(RTS, DTR) TRUE if not already set.  
*
* RETURNS: OK
*/

LOCAL STATUS AT91DBGUOpen
    (
    AT91_DBGU_CHAN * pChan 	/* pointer to channel */
    )
{
/*
    FAST int     oldlevel;	/ * current interrupt level mask * /

    oldlevel = intLock ();

    / * set RTS and DTR active * /

/ *    SNDS_REG_WRITE(pChan, SNDS_USTAT, USTAT_DTR_HIGH);	* /
    intUnlock (oldlevel);
*/

    return (OK);
}

/******************************************************************************
*
* AT91DBGUOptSet - set hardware options
*
* This routine sets up the hardware according to the specified option
* argument.  If the hardware cannot support a particular option value, then
* it should ignore that portion of the request.
*
* RETURNS: OK upon success, or EIO for invalid arguments.
*/

LOCAL int AT91DBGUOptSet
    (
    AT91_DBGU_CHAN * pChan,		/* channel */
    UINT32	newOpts          	/* new options */
    )
{
    	
	UINT32 umode =  DBGU_MR_TEST_NORMAL_MODE;
	UINT32 paritybits = DBGU_MR_PARITY_NONE;
	UINT32 rcvrEnable = DBGU_CR_RX_ENA;

    int  lvl;

    if (pChan == NULL || newOpts & 0xffffff00)
		return EIO;

    /* do nothing if options already set */
    if (pChan->options == newOpts)
		return OK;


    /* decode individual request elements */
    switch ( newOpts & (PARENB|PARODD) )
	{
	case PARENB|PARODD:
	    /* enable odd parity */
    	paritybits = DBGU_MR_PARITY_ODD;
	    break;

	case PARENB:
	    /* enable even parity */
	    paritybits = DBGU_MR_PARITY_EVEN;
	    break;

	case PARODD:
	    /* invalid mode, not normally used. */
	    break;

	default:
	case 0:
	    paritybits = DBGU_MR_PARITY_NONE;
	    break;
	}


    if ( (newOpts & CREAD) == 0 )
		rcvrEnable = DBGU_CR_RX_DIS;


    lvl = intLock ();

    /*
     * Reset the device according to dataBits, stopBits, hdweFlowCtrl,
     * rcvrEnable, and parity selections.
     */
	pChan ->regs->DBGU_CR = rcvrEnable;
    pChan ->regs->DBGU_MR = (AT91_REG)( umode | paritybits  );
  
    intUnlock (lvl);
    
	pChan->options = newOpts;
    return (OK);
}

/*******************************************************************************
*
* AT91DBGUIoctl - special device control
*
* This routine handles the IOCTL messages from the user. It supports commands 
* to get/set baud rate, mode(INT,POLL), hardware options(parity, number of 
* data bits) and modem control(RTS/CTS and DTR/DSR handshakes).
* The ioctl commands SIO_HUP and SIO_OPEN are used to implement the HUPCL(hang
* up on last close) function.
*
* As on a UNIX system, requesting a baud rate of zero is translated into
* a hangup request.  The DTR and RTS lines are dropped.  This should cause
* a connected modem to drop the connection.  The SIO_HUP command will only
* hangup if the HUPCL option is active.  The SIO_OPEN function will raise
* DTR and RTS lines whenever it is called. Use the BAUD_RATE=0 function
* to hangup when HUPCL is not active.
*
* The CLOCAL option will disable hardware flow control.  When selected,
* hardware flow control is not used.  When not selected hardware flow control
* is based on the RTS/CTS signals.  CTS is the clear to send input
* from the other end.  It must be true for this end to begin sending new
* characters.  In most drivers, the RTS signal will be assumed to be connected
* to the opposite end's CTS signal and can be used to control output from
* the other end.  Raising RTS asserts CTS at the other end and the other end
* can send data.  Lowering RTS de-asserts CTS and the other end will stop
* sending data. (This is non-EIA defined use of RTS).
*
* RETURNS: OK on success, ENOSYS on unsupported request, EIO on failed
* request.
*/

LOCAL STATUS AT91DBGUSetNewBaudRate( AT91_DBGU_CHAN * pChan,  UINT32 baud )
{
	int oldlevel;
	UINT32 count = ( MASTER_CLK + baud*8 )/(16*baud);
	if( count > 1 )
	{
		pChan->baudRate = baud;
		oldlevel = intLock ();
		pChan ->regs ->DBGU_BRGR = count;
		intUnlock (oldlevel);
	}
	return (OK);
}

LOCAL int AT91DBGUIoctl
    (
    SIO_CHAN *	pSioChan,		/* device to control */
    int		request,		/* request code */
    void *	someArg			/* some argument */
    )
{
    AT91_DBGU_CHAN *pChan = ( AT91_DBGU_CHAN * )pSioChan;
    int     oldlevel;		/* current interrupt level mask */
    int     arg = (int)someArg;
	DBGU_S* udev = pChan ->regs;
    
    switch (request)
	{
	case SIO_BAUD_SET:

	    /*
	     * like unix, a baud request for 0 is really a request to
	     * hangup.
	     */
	    if (arg == 0)
			return AT91DBGUHup (pChan);

	    /*
	     * Set the baud rate. Return EIO for an invalid baud rate, or
	     * OK on success.
	     */
	    if (arg < AT91DBGU_BAUD_MIN || arg > AT91DBGU_BAUD_MAX )
	    {
			return (EIO);
		}

		/* Calculate the baud rate constant for the new baud rate */
		switch(arg)
		{
		case 1200:
		case 2400:
		case 4800:
		case 9600:
		case 19200:
		case 38400:
		case 57600:
		case 115200:
		case 230400:
		case 460800:
			return ( AT91DBGUSetNewBaudRate( pChan, arg ) );
			
		default:
		}	
		
		return(OK);

	case SIO_BAUD_GET:

		/* Get the baud rate and return OK */
		*(int *)arg = pChan->baudRate;	
		return (OK);

	case SIO_MODE_SET:

	    /*
	     * Set the mode (e.g., to interrupt or polled). Return OK
	     * or EIO for an unknown or unsupported mode.
	     */
	    return ( AT91DBGUModeSet (pChan, arg) );

	case SIO_MODE_GET:

	    /* Get the current mode and return OK.  */
		if( pChan ->intrmode )
			*(int *)arg = SIO_MODE_INT;
		else
			*(int *)arg = SIO_MODE_POLL;
	    return (OK);

	case SIO_AVAIL_MODES_GET:

	    /* Get the available modes and return OK.  */
	    *(int *)arg = SIO_MODE_INT | SIO_MODE_POLL; 
	    return (OK);

	case SIO_HW_OPTS_SET:

	    /*
	     * Optional command to set the hardware options (as defined
	     * in sioLib.h).
	     * Return OK, or ENOSYS if this command is not implemented.
	     * Note: several hardware options are specified at once.
	     * This routine should set as many as it can and then return
	     * OK. The SIO_HW_OPTS_GET is used to find out which options
	     * were actually set.
	     */
	    return ( AT91DBGUOptSet (pChan, arg) );

	case SIO_HW_OPTS_GET:

	    /*
	     * Optional command to get the hardware options (as defined
	     * in sioLib.h). Return OK or ENOSYS if this command is not
	     * implemented.  Note: if this command is unimplemented, it
	     * will be assumed that the driver options are CREAD | CS8
	     * (e.g., eight data bits, one stop bit, no parity, ints enabled).
	     */
	    *(int *)arg = pChan->options;
	    return (OK);

	case SIO_HUP:

        /* check if hupcl option is enabled */
		if ( pChan->options & HUPCL ) 
			return ( AT91DBGUHup (pChan) );
		return (OK);

	case SIO_OPEN:
		return ( AT91DBGUOpen (pChan) ); /* always open */

	default:
	    return (ENOSYS);
	}
    return (ENOSYS);
}

