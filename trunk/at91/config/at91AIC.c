/* at91AIC.c - AT91RM9200 AIC driver */

/*
modification history
--------------------

*/

/*

DESCRIPTION

*/

#include "intLib.h"

/* forward declarations */

STATUS	AT91IntLvlVecChk (int*, int*);
/*STATUS  AT91IntLvlVecAck (int, int);*/

STATUS	AT91IntLvlEnable (int);
STATUS	AT91IntLvlDisable (int);

/*******************************************************************************
*
* At91IntDevInit - initialize the interrupt controller
*
* This routine will initialize the interrupt controller device, disabling all
* interrupt sources.  It will also connect the device driver specific routines
* into the architecture level hooks.  If the BSP needs to create a wrapper
* routine around any of the arhitecture level routines, it should install the
* pointer to the wrapper routine after calling this routine.
*
* RETURNS: N/A
*/

void At91IntDevInit (void)
{
	int i;
	AIC_S* paic = (AIC_S*)AIC_BASE_ADDR;

	/* dummy read result nIRQ, nFIQ to deassert state */
	i = paic ->AIC_IVR;
	i = paic ->AIC_FVR;
	
	/*
	 *	setup the mode and vector for each interrupt
	 *	because we can not use the AIC automatic vector branch, we set
	 *	each vector to it's interrupt number, and we are easy to get interrupt number
	 *	when sndsIntLvlVecChk called.
	 *	set each interrupt mode to priority 0( lowest level ), edge trig mode
	 */
	for( i = 0; i<AIC_INT_NUM_LEVELS; i++ )
	{
		paic ->AIC_SMR[i] = ( 7<<(AIC_SMR_PRIO_BIT) );
		paic ->AIC_SVR[i] = (AT91_REG)(i|0x20000000);
	}
	
	/* SUPRISE  INTERRUPT VECTOR, WE SET TO A LARGE VALUE */
	paic ->AIC_SPU = (AIC_INT_NUM_LEVELS+10)|0x20000000;
	
	/* CURRENT WE  USE PROTECT MODE, if used, IVR must be writed just after it readed */
	paic ->AIC_DCR = 1;
	
	/* faster forceing, we do not use any interrupt routed to FIRQ */
	paic ->AIC_FFER = 0;
	paic ->AIC_FFDR = 0xFFFFFFFE; /* leave last bit to 0, It's FIQ */
	
	
	for( i = 0; i<8; i++ )
		paic ->AIC_EOICR = 0;
	
	paic ->AIC_ICCR = 0xFFFFFFFF;

	/* install the driver routines in the architecture hooks */
    sysIntLvlVecChkRtn	= AT91IntLvlVecChk;
    sysIntLvlEnableRtn	= AT91IntLvlEnable;
    sysIntLvlDisableRtn	= AT91IntLvlDisable;
/*
    sysIntLvlVecAckRtn	= AT91IntLvlVecAck;
*/
	
}

/*******************************************************************************
*
* AT91IntLvlVecChk - check for and return any pending interrupts
*
* This routine interrogates the hardware to determine the highest priority
* interrupt pending.  It returns the vector associated with that interrupt, and
* also the interrupt priority level prior to the interrupt (not the
* level of the interrupt).  The current interrupt priority level is then
* raised to the level of the current interrupt so that only higher priority
* interrupts will be accepted until this interrupt is finished.
*
* The return value ERROR indicates that no pending interrupt was found and
* that the level and vector values were not returned.
*
* RETURNS: OK or ERROR if no interrupt is pending.
*/

STATUS  AT91IntLvlVecChk
    (
    int* pLevel,  /* ptr to receive old interrupt level */
    int* pVector  /* ptr to receive current interrupt vector */
    )
{

	AIC_S* paic = (AIC_S*)AIC_BASE_ADDR;
	UINT32 newLevel;
	newLevel = paic ->AIC_IVR;
	paic ->AIC_IVR = newLevel;
	newLevel &= 0xFF;
	paic ->AIC_EOICR = 0xFFFFFFFF;

	if( newLevel >= AIC_INT_NUM_LEVELS )
		return ERROR;
	newLevel &= (AIC_INT_NUM_LEVELS-1);
	*pVector = newLevel;
	return OK;
}

/*******************************************************************************
*
* AT91IntLvlEnable - enable a single interrupt level
*
* Enable a specific interrupt level.  The enabled level will be allowed to
* generate an interrupt, when the overall interrupt level is set below the
* specified level.  Without being enabled, the interrupt is blocked regardless
* of the overall interrupt level setting.
*
* RETURNS: OK or ERROR if the specified level cannot be enabled.
*/

STATUS  AT91IntLvlEnable
    (
    int level  /* level to be enabled */
    )
{
    if ( ((UINT32)level) >= AIC_INT_NUM_LEVELS )
		return ERROR;
	((AIC_S*)AIC_BASE_ADDR)->AIC_IECR = (1 << level);

	return OK;
}

/*******************************************************************************
*
* AT91IntLvlDisable - disable a single interrupt level
*
* Disable a specific interrupt level.  The disabled level is prevented
* from generating an interrupt even if the overall interrupt level is set
* below the specified level.
*
* RETURNS: OK or ERROR, if the specified interrupt level cannot be disabled.
*/

STATUS  AT91IntLvlDisable
    (
    int level  /* level to be disabled */
    )
{
    if ( ((UINT32)level) >= AIC_INT_NUM_LEVELS )
		return ERROR;
	((AIC_S*)AIC_BASE_ADDR)->AIC_IDCR = (1 << level);
}


/*******************************************************************************
*
* AT91IntLvlVecAck - acknowledge the current interrupt
*
* Acknowledge the current interrupt cycle.  The level and vector values are
* those generated during the IntLvlVecChk() routine for this interrupt
* cycle.  The basic action is to reset the current interrupt and return
* the interrupt level to its previous setting.  Note that the AMBA interrupt
* controller does not need an acknowledge cycle.
*
* RETURNS: OK or ERROR if a hardware fault is detected.
* ARGSUSED
*/

/*
STATUS  AT91IntLvlVecAck
    (
    int level,	/ * old interrupt level to be restored * /
    int vector	/ * current interrupt vector, if needed * /
    )
{

	/ *((AIC_S*)AIC_BASE_ADDR) ->AIC_EOICR = 0xFFFFFFFF;* /	/ * ack interrupt, int LEVEL stack adjuested * /
    return OK;
}
*/
