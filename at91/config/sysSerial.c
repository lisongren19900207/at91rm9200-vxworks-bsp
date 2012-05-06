/* sysSerial.c - ARM AT91RM9200 board serial device initialization */

/* Copyright 1999-2000 ARM Limited */
/* Copyright 1999-2000 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
*/

/*

SEE ALSO:
.I "ARM Integrator AP User Guide (ARM DUI 0098)."
.I "UART (PL010) Technical Reference Manual (ARM DDI 0139)."

*/



/* device initialization structure */

typedef struct
{
    UINT32	vector;
    UART_S *baseAdrs;
    UINT32	intLevel;
	UINT32	clkdiv;
} SYS_AT91_CHAN_PARAS;


/* Local data structures */

LOCAL SYS_AT91_CHAN_PARAS devParas[] =
{
	{ INT_VEC_UART0, (UART_S *)UART0_BASE_ADDR, INT_LVL_UART0, 1 },
	{ INT_VEC_UART1, (UART_S *)UART1_BASE_ADDR, INT_LVL_UART1, 1 },
	{ INT_VEC_UART2, (UART_S *)UART2_BASE_ADDR, INT_LVL_UART2, 1 },
	{ INT_VEC_UART3, (UART_S *)UART3_BASE_ADDR, INT_LVL_UART3, 1 },
};

LOCAL AT91_DBGU_CHAN at91DBGUChan;
LOCAL AT91_CHAN at91UartChan[ N_SIO_CHANNELS ];


/*
 * Array of pointers to all serial channels configured in system.
 * See sioChanGet(). It is this array that maps channel pointers
 * to standard device names.  The first entry will become "/tyCo/0",
 * the second "/tyCo/1", and so forth.
 */

SIO_CHAN *sysSioChans [] =
{
	&at91DBGUChan.sio,	 
	&at91UartChan[0].sio,
	&at91UartChan[1].sio,
	/*
    &at91UartChan[2].sio, 
    &at91UartChan[3].sio, 
	*/
};


/* forward declarations */

/******************************************************************************
*
* sysSerialHwInit - initialize the BSP serial devices to a quiescent state
*
* This routine initializes the BSP serial device descriptors and puts the
* devices in a quiesent state.  It is called from sysHwInit() with
* interrupts locked.
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit()
*/



void sysSerialHwInit (void)
{
    int i;

	at91DBGUChan.regs = (DBGU_S*)DBGU_BASE_ADDR;
	at91DBGUChan.baudRate = CONSOLE_BAUD_RATE;
	AT91DBGUDevInit( &at91DBGUChan );
	

    for (i = 0; i < N_SIO_CHANNELS; i++)
	{
		at91UartChan[i].regs = devParas[i].baseAdrs;
		at91UartChan[i].baudRate = CONSOLE_BAUD_RATE;
		at91UartChan[i].clkdiv = devParas[i].clkdiv;

		/* recave and send always use same interrupt*/
		at91UartChan[i].level = devParas[i].intLevel;
		

		/*
		 * Initialise driver functions, getTxChar, putRcvChar and channelMode
		 * and initialise UART
		 */
		AT91UartDevInit (&at91UartChan[i]);
	}
}

/******************************************************************************
*
* sysSerialHwInit2 - connect BSP serial device interrupts
*
* This routine connects the BSP serial device interrupts.  It is called from
* sysHwInit2().  Serial device interrupts could not be connected in
* sysSerialHwInit() because the kernel memory allocator was not initialized
* at that point, and intConnect() may call malloc().
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit2()
*/

void sysSerialHwInit2 (void)
{
    int i;

	AT91DBGUDevInit2( &at91DBGUChan );

    for (i = 0; i < N_SIO_CHANNELS; i++)
	{
		/*
		 * Connect and enable the interrupt.
		 * We would like to check the return value from this and log a message
		 * if it failed. However, logLib has not been initialised yet, so we
		 * cannot log a message, so there's little point in checking it.
		 * URAT still in quiet state
		 */
		(void) intConnect ( INUM_TO_IVEC(devParas[i].vector),
				   AT91UartInt, (UINT32) &at91UartChan[i] );
		
		intEnable (devParas[i].intLevel);
		AT91UartDevInit2( &at91UartChan[i] );
	}
}

/******************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine returns a pointer to the SIO_CHAN device associated with
* a specified serial channel.  It is called by usrRoot() to obtain
* pointers when creating the system serial devices '/tyCo/x'.  It is also
* used by the WDB agent to locate its serial channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*/

SIO_CHAN * sysSerialChanGet
    (
    int channel         /* serial channel */
    )
{
    if (channel < 0 || channel >= (int)(NELEMENTS(sysSioChans)))
		return (SIO_CHAN *)ERROR;

    return sysSioChans[channel];
}

/******************************************************************************
*
* sysSerialReset - reset the sio devices to a quiet state
*
* Reset all devices to prevent them from generating interrupts.
*
* This is called from sysToMonitor to shutdown the system gracefully before
* transferring to the boot ROMs.
*
* RETURNS: N/A.
*/

void sysSerialReset (void)
{
    int i;
	
	at91DBGUChan.regs ->DBGU_IDR = 0xFFFFFFFF;
	at91DBGUChan.regs ->DBGU_CR = UART_CR_RESET_RECV | UART_CR_RESET_TRAN | UART_CR_RX_DIS | UART_CR_TX_DIS;

    for (i = 0; i < N_SIO_CHANNELS ; i++)
	{
		/* disable serial interrupts */
		intDisable (devParas[i].intLevel);
		
		/*
		 *	disable all interrupt source first
		 */
		at91UartChan[i].regs->US_IDR = 0xFFFFFFFF;
		
		/*
		 *	set all channel to quiet state.
		 */
		at91UartChan[i].regs->US_CR = UART_CR_RESET_RECV | UART_CR_RESET_TRAN | UART_CR_RX_DIS | UART_CR_TX_DIS;
		at91UartChan[i].regs->US_MR = UART_MR_CHAR_8BIT | UART_MR_STOP_1BIT | UART_MR_PARITY_NONE
					| UART_MR_CLK_USE_MCKDIV| UART_MR_TEST_NORMAL_MODE| UART_MR_UART_MODE_NORMAL;
	}
}


