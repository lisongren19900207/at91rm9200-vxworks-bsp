/* at91TIMER.c - AT91RM9200 Timer library */

/*
modification history
--------------------
2012/4/28 this file is modified form VxWorks demo bsp integrator920t

*/

/*

DESCRIPTION

INCLUDES:
at91TIMER.h
timestampDev.h
*/

/* includes */

#include "drv/timer/timerDev.h"
#include "drv/timer/timestampDev.h"

/* defines */

/* The default is to assume memory mapped I/O */



#ifndef AT91_TIMER_INT_ENABLE
#define AT91_TIMER_INT_ENABLE(level) intEnable(level)
#endif

#ifndef AT91_TIMER_INT_DISABLE
#define AT91_TIMER_INT_DISABLE(level) intDisable(level)
#endif


/* locals */

LOCAL FUNCPTR sysClkRoutine	= NULL; /* routine to call on clock tick */
LOCAL int sysClkArg		= 0; /* its argument */
LOCAL int sysClkRunning		= FALSE;
LOCAL int sysClkTicksPerSecond	= 60;

LOCAL FUNCPTR sysAuxClkRoutine	= NULL;
LOCAL int sysAuxClkArg		= 0;
LOCAL int sysAuxClkRunning	= FALSE;
LOCAL int sysAuxClkTicksPerSecond = 100;
LOCAL int sysAuxClkTicks;

#ifdef INCLUDE_TIMESTAMP

LOCAL BOOL	sysTimestampRunning	= FALSE; /* running flag */
LOCAL int	sysTimestampPeriodValue	= 0;  /* Max counter value */
LOCAL FUNCPTR	sysTimestampRoutine	= NULL;  /* routine to call on intr */
LOCAL int	sysTimestampArg		= 0;     /* arg for routine */
      void	sysTimestampInt (void);		 /* forward declaration */

#endif  /* INCLUDE_TIMESTAMP */

/*******************************************************************************
* at91SysTimer
*
*/
void InitSystemTimer()
{
	SYSTIMER_S* pst = (SYSTIMER_S*)SYSTIMER_BASE_ADDR;
	pst ->ST_CR = ST_CR_WDRST;
	pst ->ST_PIMR = ( UINT32 )( SLOW_RATE_CLK / DEFAULT_SYS_CLK_RATE );
	pst ->ST_WDMR = 0x0;
	
	/*
	 *	real time prescale set to only 1, no prescale
	 */
	pst ->ST_RTMR = ST_RTMT_RT_PRESCALE;
	pst ->ST_IDR = (ST_IE_DR_ALARMS | ST_IE_DR_PITS | ST_IE_DR_RTTINC | ST_IE_DR_WDOVF );
}

void InitSystemTimer2()
{
	SYSTIMER_S* pst = (SYSTIMER_S*)SYSTIMER_BASE_ADDR;
	
	/*
	 *	enable interrupt of system period timer
	 */
	pst ->ST_IER = ST_IE_DR_PITS;
}

/*******************************************************************************
*
* sysClkInt - interrupt level processing for system clock
*
* This routine handles an system clock interrupt.  It acknowledges the
* interrupt and calls the routine installed by sysClkConnect().
*/

void TimerCounter_Global_Init()
{
	/* set two block extern clock input to none ? */
	((AT91_REG*)TIMER0_BASE_ADDR)[ TC_BMR_OFFSET/4 ] = TC0_BMR_VALUE;
	((AT91_REG*)TIMER3_BASE_ADDR)[ TC_BMR_OFFSET/4 ] = TC0_BMR_VALUE;

	/* set all timer to be quite state: disabled wave mode, MASTER CLOCK/128 as clock source */
	( (TimerCounter*)TIMER0_BASE_ADDR ) ->TC_CCR = TIMER0_CCR_CLKDIS;
	( (TimerCounter*)TIMER0_BASE_ADDR ) ->TC_CMR = ((1<<15)|(0x3));

	( (TimerCounter*)TIMER1_BASE_ADDR ) ->TC_CCR = TIMER0_CCR_CLKDIS;
	( (TimerCounter*)TIMER1_BASE_ADDR ) ->TC_CMR = ((1<<15)|(0x3));

	( (TimerCounter*)TIMER2_BASE_ADDR ) ->TC_CCR = TIMER0_CCR_CLKDIS;
	( (TimerCounter*)TIMER2_BASE_ADDR ) ->TC_CMR = ((1<<15)|(0x3));

	( (TimerCounter*)TIMER3_BASE_ADDR ) ->TC_CCR = TIMER0_CCR_CLKDIS;
	( (TimerCounter*)TIMER3_BASE_ADDR ) ->TC_CMR = ((1<<15)|(0x3));
	
	( (TimerCounter*)TIMER4_BASE_ADDR ) ->TC_CCR = TIMER0_CCR_CLKDIS;
	( (TimerCounter*)TIMER4_BASE_ADDR ) ->TC_CMR = ((1<<15)|(0x3));
	
	( (TimerCounter*)TIMER5_BASE_ADDR ) ->TC_CCR = TIMER0_CCR_CLKDIS;
	( (TimerCounter*)TIMER5_BASE_ADDR ) ->TC_CMR = ((1<<15)|(0x3));
}

/*
 *	this routine is used for 
 *	1. system period timer interrupt
 *	2. DBGU interrupt
 *
 */
extern AT91_DBGU_CHAN at91DBGUChan;
void sysClkInt (void)
{
	static UINT32 counter;
	UINT32 status;
	SYSTIMER_S* pst = (SYSTIMER_S*)( SYSTIMER_BASE_ADDR );
	DBGU_S* pdbgu = ( DBGU_S* )( DBGU_BASE_ADDR );
	
	/*
	 *	check vxworks system timer interrupt
	 */
	status = pst ->ST_SR;
	if( ( sysClkRunning == TRUE ) && (  status & ST_SR_PITS ) && ( sysClkRoutine != NULL ) )
	{
		/* call system clock service routine */
		(* sysClkRoutine) (sysClkArg);
	}

	/*
	 *	check DBGU interrupt
	 */
	if( at91DBGUChan.intrmode )
	{
		status = ( (pdbgu ->DBGU_CSR)
			&( DBGU_CSR_OVRE | DBGU_CSR_FRAME | DBGU_CSR_PARE | DBGU_CSR_TXRDY | DBGU_CSR_RXRDY ) );
		if( status ) AT91DBGUInt( &at91DBGUChan );
	}

}


/*******************************************************************************
*
* sysClkConnect - connect a routine to the system clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* clock interrupt.  Normally, it is called from usrRoot() in usrConfig.c to 
* connect usrClock() to the system clock interrupt.
*
* RETURN: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), usrClock(), sysClkEnable()
*/

STATUS sysClkConnect
(
	FUNCPTR routine,	/* routine to be called at each clock interrupt */
	int arg		/* argument with which to call routine */
)
{
	static BOOL beenHere = FALSE;

	if (!beenHere)
	{
		beenHere = TRUE;
		sysHwInit2 ();
	}

	sysClkRoutine   = NULL;
	sysClkArg	    = arg;
	sysClkRoutine   = routine;

	return (OK);
}


/*******************************************************************************
*
* sysClkDisable - turn off system clock interrupts
*
* This routine disables system clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysClkEnable()
*/

void sysClkDisable (void)
{
    if (sysClkRunning)
	{
		/*
		 *	disable ST period interrupt
		 */
		( (SYSTIMER_S*)( SYSTIMER_BASE_ADDR ) ) ->ST_IDR = 0xFFFFFFFF;
		sysClkRunning = FALSE;
	}
}


/*******************************************************************************
*
* sysClkEnable - turn on system clock interrupts
*
* This routine enables system clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysClkConnect(), sysClkDisable(), sysClkRateSet()
*/

void sysClkEnable (void)
{
	UINT32 sysClkTicks;
	SYSTIMER_S* pst;

	if (!sysClkRunning)
	{
		/*
		 * Calculate the number of ticks of the timer clock that this
		 * period requires.  Do this once, here, so that the timer interrupt
		 * routine does not need to perform a division.
		 */
		sysClkTicks = ( SLOW_RATE_CLK / sysClkTicksPerSecond );
		pst = (SYSTIMER_S*)( SYSTIMER_BASE_ADDR );
		pst ->ST_PIMR = sysClkTicks;
		
		/*
		 *	dummy read status reg to clear interrupt status
		 */
		sysClkTicks = pst ->ST_SR;
		pst ->ST_IER = ST_IE_DR_PITS;

		sysClkRunning = TRUE;
	}
}


/*******************************************************************************
*
* sysClkRateGet - get the system clock rate
*
* This routine returns the system clock rate.
*
* RETURNS: The number of ticks per second of the system clock.
*
* SEE ALSO: sysClkEnable(), sysClkRateSet()
*/

int sysClkRateGet (void)
{
	return (sysClkTicksPerSecond);
}


/*******************************************************************************
*
* sysClkRateSet - set the system clock rate
*
* This routine sets the interrupt rate of the system clock.
* It is called by usrRoot() in usrConfig.c.
*
* RETURNS: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
* SEE ALSO: sysClkEnable(), sysClkRateGet()
*/

STATUS sysClkRateSet
    (
    int ticksPerSecond	    /* number of clock interrupts per second */
    )
{
	if (ticksPerSecond < SYS_CLK_RATE_MIN || ticksPerSecond > SYS_CLK_RATE_MAX)
		return (ERROR);

	if ( sysClkRunning )
	{
		sysClkDisable ();
		sysClkTicksPerSecond = ticksPerSecond;
		sysClkEnable ();
	}
	return (OK);
}

/*******************************************************************************
*
* sysAuxClkInt - handle an auxiliary clock interrupt
*
* This routine handles an auxiliary clock interrupt.  It acknowledges the
* interrupt and calls the routine installed by sysAuxClkConnect().
*
* RETURNS: N/A
*/

void sysAuxClkInt (void)
{
    /* call auxiliary clock service routine */
	int dummy = ((TimerCounter*)TIMER0_BASE_ADDR)->TC_SR;
	
    if (sysAuxClkRoutine != NULL)
		(*sysAuxClkRoutine) (sysAuxClkArg);
}

/*******************************************************************************
*
* sysAuxClkConnect - connect a routine to the auxiliary clock interrupt
*
* This routine specifies the interrupt service routine to be called at each
* auxiliary clock interrupt.  It does not enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), sysAuxClkEnable()
*/

STATUS sysAuxClkConnect
    (
    FUNCPTR routine,    /* routine called at each aux clock interrupt */
    int arg             /* argument to auxiliary clock interrupt routine */
    )
{
    sysAuxClkRoutine	= NULL;
    sysAuxClkArg	= arg;
    sysAuxClkRoutine	= routine;

    return (OK);
}

/*******************************************************************************
*
* sysAuxClkDisable - turn off auxiliary clock interrupts
*
* This routine disables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysAuxClkEnable()
*/

void sysAuxClkDisable (void)
{
	if (sysAuxClkRunning)
	{
		AT91_TIMER_INT_DISABLE ( INT_LVL_TC0 );
		((TimerCounter*)TIMER0_BASE_ADDR)->TC_IDR = (AT91_REG)( TIMER0_IDR_VALUE | TIMER0_IER_VALUE );
		((TimerCounter*)TIMER0_BASE_ADDR)->TC_CCR = (AT91_REG)TIMER0_CCR_CLKDIS;
		sysAuxClkRunning = FALSE;
	}
}

/*******************************************************************************
*
* sysAuxClkEnable - turn on auxiliary clock interrupts
*
* This routine enables auxiliary clock interrupts.
*
* RETURNS: N/A
*
* SEE ALSO: sysAuxClkConnect(), sysAuxClkDisable(), sysAuxClkRateSet()
*/

void sysAuxClkEnable (void)
{
    UINT32 sysClkTicks;
	TimerCounter* ptc0 = (TimerCounter*)TIMER0_BASE_ADDR;
/*
    static connected = FALSE;

    if (!connected)
	{
		intConnect ( INT_VEC_TC1, sysAuxClkInt, 0 );
		connected = TRUE;
	}
*/

    if (!sysAuxClkRunning)
    {
		/*
		 * Calculate the number of ticks of the timer clock that this
		 * period requires.  Do this once, here, so that the timer interrupt
		 * routine does not need to perform a division.
		 */

		/* disable timer interrupt in the timer */
		ptc0 ->TC_CCR = (AT91_REG)TIMER0_CCR_CLKDIS;
		
		sysClkTicks = ( TIMER0_CLOCK / sysAuxClkTicksPerSecond );

		/*
		 * Load the match register with a new value calculated by
		 * adding the ticks per interrupt to the current value of the
		 * counter register.  Note that this may wraparound to a value
		 * less than the current counter value but that's OK.
		 */
		ptc0 ->TC_CMR = (AT91_REG)TIMER0_TCMR_VALUE;
		ptc0 ->TC_RA = (AT91_REG)(0xFFFF);
		ptc0 ->TC_RB = (AT91_REG)(0xFFFF);
		ptc0 ->TC_RC = (AT91_REG)sysClkTicks;
		ptc0 ->TC_IER = (AT91_REG)TIMER0_IER_VALUE;
		ptc0 ->TC_IDR = (AT91_REG)TIMER0_IDR_VALUE;
		sysClkTicks = ptc0 ->TC_SR;
		ptc0->TC_CCR = (AT91_REG)TIMER0_CCR_VALUE;		

		/* enable clock interrupt in interrupt controller */
		AT91_TIMER_INT_ENABLE( INT_LVL_TC0 );
		sysAuxClkRunning = TRUE;
	}
}

/*******************************************************************************
*
* sysAuxClkRateGet - get the auxiliary clock rate
*
* This routine returns the interrupt rate of the auxiliary clock.
*
* RETURNS: The number of ticks per second of the auxiliary clock.
*
* SEE ALSO: sysAuxClkEnable(), sysAuxClkRateSet()
*/

int sysAuxClkRateGet (void)
{
    return ( sysAuxClkTicksPerSecond );
}

/*******************************************************************************
*
* sysAuxClkRateSet - set the auxiliary clock rate
*
* This routine sets the interrupt rate of the auxiliary clock.  It does not
* enable auxiliary clock interrupts.
*
* RETURNS: OK, or ERROR if the tick rate is invalid or the timer cannot be set.
*
* SEE ALSO: sysAuxClkEnable(), sysAuxClkRateGet()
*/

STATUS sysAuxClkRateSet
    (
    int ticksPerSecond  /* number of clock interrupts per second */
    )
{
    if ( ( ticksPerSecond < AUX_CLK_RATE_MIN ) || ( ticksPerSecond > AUX_CLK_RATE_MAX ) )
        return (ERROR);

	sysAuxClkTicksPerSecond = ticksPerSecond;
	
    if (sysAuxClkRunning)
	{
		sysAuxClkDisable ();
		sysAuxClkEnable ();
	}
    return (OK);
}

#ifdef  INCLUDE_TIMESTAMP

/*******************************************************************************
*
* sysTimestampInt - timestamp timer interrupt handler
*
* This rountine handles the timestamp timer interrupt.  A user routine is
* called, if one was connected by sysTimestampConnect().
*
* RETURNS: N/A
*
* SEE ALSO: sysTimestampConnect()
*/


void sysTimestampInt (void)
{
	int dummy = ((TimerCounter*)TIMER2_BASE_ADDR)->TC_SR;
	
	if ( sysTimestampRunning && sysTimestampRoutine != NULL)
		(*sysTimestampRoutine)(sysTimestampArg);
}


/*******************************************************************************
*
* sysTimestampConnect - connect a user routine to the timestamp timer interrupt
*
* This routine specifies the user interrupt routine to be called at each
* timestamp timer interrupt.  It does not enable the timestamp timer itself.
*
* RETURNS: OK, or ERROR if sysTimestampInt() interrupt handler is not used.
*/

STATUS sysTimestampConnect
    (
    FUNCPTR routine,    /* routine called at each timestamp timer interrupt */
    int arg     /* argument with which to call routine */
    )
{
	sysTimestampRoutine = NULL;
	sysTimestampArg = arg;
	sysTimestampRoutine = routine;
	return OK;	/* System clock tick specifies a rollover event */
}

/*******************************************************************************
*
* sysTimestampEnable - initialize and enable the timestamp timer
*
* This routine connects the timestamp timer interrupt and initializes the
* counter registers.  If the timestamp timer is already running, this routine
* merely resets the timer counter.
*
* The rate of the timestamp timer should be set explicitly within the BSP,
* in the sysHwInit() routine.  This routine does not intialize the timer
* rate.
*
* RETURNS: OK, or ERROR if hardware cannot be enabled.
*/

STATUS sysTimestampEnable (void)
{
	TimerCounter* pct1 = (TimerCounter*)TIMER1_BASE_ADDR;
	TimerCounter* pct2 = (TimerCounter*)TIMER2_BASE_ADDR;
	UINT32 dummy;

/*
    static BOOL connected = FALSE;

    if (!connected)
	{
		connected = TRUE;
	}
*/

    if ( !sysTimestampRunning )
	{
/*
		( (SYSTIMER_S*)SYSTIMER_BASE_ADDR ) ->ST_RTMR = ST_RTMT_RT_PRESCALE;
*/

		pct1 ->TC_CCR = (AT91_REG)TIMER1_CCR_CLKDIS;
		pct2 ->TC_CCR = (AT91_REG)TIMER2_CCR_CLKDIS;

		/*
		 *	TIMER 1 free run from 0 - 0xFFFF, and have no trig
		 *	TIOA1 output period is 0-0xFFFF, but first assert at 0xFF00, deassert at 0xFFF0
		 *	TIMER1 have no interrupt
		 */
		pct1 ->TC_CMR = (AT91_REG)TIMER1_TCMR_VALUE;
		pct2 ->TC_CMR = (AT91_REG)TIMER2_TCMR_VALUE;

		pct1 ->TC_RA = (AT91_REG)(TIMER1_RC_VALUE/2);
		pct1 ->TC_RB = (AT91_REG)(TIMER1_RC_VALUE/2);
		pct1 ->TC_RC = (AT91_REG)(TIMER1_RC_VALUE);
		pct1 ->TC_IER = (AT91_REG)0;
		pct1 ->TC_IDR = (AT91_REG)0xFFFFFFFF;

		pct2 ->TC_RA = (AT91_REG)(0xFFFF);
		pct2 ->TC_RB = (AT91_REG)(0xFFFF);
		pct2 ->TC_RC = (AT91_REG)(TIMER2_RC_VALUE);
		pct2 ->TC_IER = (AT91_REG)TIMER2_IER_VALUE;
		pct2 ->TC_IDR = (AT91_REG)TIMER2_IDR_VALUE;
		
		dummy = pct1 ->TC_SR;
		dummy = pct2 ->TC_SR;
		
		
		/* start the Timer1 */
		pct1->TC_CCR = (AT91_REG)TIMER1_CCR_VALUE;		

		/* start the Timer2 */
		pct2->TC_CCR = (AT91_REG)TIMER2_CCR_VALUE;		

		/* enable clock interrupt in interrupt controller */
		AT91_TIMER_INT_ENABLE( INT_LVL_TC2 );

		sysTimestampRunning = TRUE;
	}

    return (OK);
}

/*******************************************************************************
*
* sysTimestampDisable - disable the timestamp timer
*
* This routine disables the timestamp timer.  Interrupts are not disabled,
* although the tick counter will not increment after the timestamp timer
* is disabled, thus interrupts will no longer be generated.
*
* RETURNS: OK, or ERROR if timer cannot be disabled.
*/

STATUS sysTimestampDisable (void)
{
    if (sysTimestampRunning)
	{

		AT91_TIMER_INT_DISABLE ( INT_LVL_TC2 );
		((TimerCounter*)TIMER2_BASE_ADDR)->TC_IDR = (AT91_REG)0xFFFFFFFF;
		((TimerCounter*)TIMER1_BASE_ADDR)->TC_CCR = (AT91_REG)TIMER1_CCR_CLKDIS;
		((TimerCounter*)TIMER2_BASE_ADDR)->TC_CCR = (AT91_REG)TIMER2_CCR_CLKDIS;
        sysTimestampRunning = FALSE;
	}
    return (OK);
}

/*******************************************************************************
*
* sysTimestampPeriod - get the timestamp timer period
*
* This routine returns the period of the timestamp timer in ticks.
* The period, or terminal count, is the number of ticks to which the timestamp
* timer will count before rolling over and restarting the counting process.
*
* RETURNS: The period of the timestamp timer in counter ticks.
*/

UINT32 sysTimestampPeriod (void)
{
	sysTimestampPeriodValue =  ((UINT32)TIMER2_RC_VALUE);
		/*( ((UINT32)TIMER2_RC_VALUE)*(UINT32)0x10000 )*/;
    return ( sysTimestampPeriodValue );
}

/*******************************************************************************
*
* sysTimestampFreq - get the timestamp timer clock frequency
*
* This routine returns the frequency of the timer clock, in ticks per second.
*
* RETURNS: The timestamp timer clock frequency, in ticks per second.
*/

UINT32 sysTimestampFreq (void)
{
    return (  TIMER1_CLOCK/TIMER1_RC_VALUE );
}

/*******************************************************************************
*
* sysTimestamp - get the timestamp timer tick count
*
* This routine returns the current value of the timestamp timer tick counter.
* The tick count can be converted to seconds by dividing by the return of
* sysTimestampFreq().
*
* This routine should be called with interrupts locked.  If interrupts are
* not already locked, sysTimestampLock() should be used instead.
*
* RETURNS: The current timestamp timer tick count.
*
* SEE ALSO: sysTimestampLock()
*/

UINT32 sysTimestamp (void)
{
	return (((TimerCounter*)TIMER2_BASE_ADDR) ->TC_CV);
/*
    UINT32 count1, count2;
	TimerCounter* pct1 = (TimerCounter*)TIMER1_BASE_ADDR;
	TimerCounter* pct2 = (TimerCounter*)TIMER2_BASE_ADDR;
	
	
/ *
	 *	stop the timer 1 by set clk source to xc2 : none
* /
	 


	pct1 ->TC_CMR = (AT91_REG)TIMER1_TCMR_PAUSE_VALUE;


	count1 = pct1 ->TC_CV;
	count2 = pct2 ->TC_CV;


	pct1 ->TC_CMR = TIMER1_TCMR_VALUE;


	

/ *
	count = ( (SYSTIMER_S*)SYSTIMER_BASE_ADDR ) -> ST_CRTR;
	count &= (ST_RTT_MAX_COUNT_VALUE-1);
* /



    return ((count2<<16)|( count1&0xFFFF ));

	return count2;*/

}

/*******************************************************************************
*
* sysTimestampLock - get the timestamp timer tick count
*
* This routine returns the current value of the timestamp timer tick counter.
* The tick count can be converted to seconds by dividing by the return of
* sysTimestampFreq().
*
* This routine locks interrupts for cases where it is necessary to stop the
* tick counter in order to read it, or when two independent counters must
* be read.  If interrupts are already locked, sysTimestamp() should be
* used instead.
*
* RETURNS: The current timestamp timer tick count.
*
* SEE ALSO: sysTimestamp()
*/

UINT32 sysTimestampLock (void)
{
	return (((TimerCounter*)TIMER2_BASE_ADDR) ->TC_CV);
	
/*
	UINT32 oldlevel;
	UINT32 x;
/ *
	oldlevel = intLock();	
* /
	
    x = sysTimestamp ();

/ *
	intUnlock( oldlevel );
* /
	return x;*/

}

#endif  /* INCLUDE_TIMESTAMP */
