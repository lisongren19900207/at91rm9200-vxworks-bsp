#include "vxWorks.h"
#include "semLib.h"
#include "taskLib.h"
#include "msgQLib.h"
#include "wdLib.h"
#include "logLib.h"
#include "tickLib.h"
#include "sysLib.h"
#include "stdio.h"
#include "signal.h"
#include "sigLib.h"
#include "timers.h"

/* defines */
#define  TIMER_RELATIVE 0       /*  interval relative to the current time */
/* globals */

WDOG_ID	wdId;			/* watchdog ID */
void tickInt(int parameter)
{
	logMsg("Watchdog timer just expired\n",0,0,0,0,0,0);
}
/**
 * use watch dog timer as timeout alarm
 */
wdTest
	(
	int iteration			/* number of times through loop */
	)
{
	int loopCount = 0; 			/* number of times through wdTest */

	/* create watchdog timer */	
	if((wdId = wdCreate ()) == NULL)
		return (ERROR);
	printf("wdId=%d\n",wdId);
	/* test watchdog timer, print msg once when timeout */
	if(wdStart (wdId, sysClkRateGet() * 5, (FUNCPTR)tickInt, 1) == ERROR)
		return (ERROR);
	
	FOREVER {
    		printf ("\nParent wdTest has just completed loop number %d\n",
		loopCount);
		taskDelay(sysClkRateGet() * 10);
    		loopCount++;
		if(loopCount>iteration)
			break;
	}	
}

/*****************************************************************************
 * sigHandler - Signal handler gets executed when SIGALRM signal is generated 
 *              by a POSIX timer.
 */

void sigHandler 
    (
     int sig,     /* signal number */
     int code,    /* additional code */
     SIGCONTEXT *sigContext /* context of task before signal */
     )
     {
         if (sig == SIGALRM)
         {
             logMsg ("Signal SIGALRM raised\n", 0, 0, 0, 0, 0, 0);
         }

     }
STATUS sigTest
	(
	int iteration			/* number of times through loop */
	)
	{
		int loopCount = 0; 			 /* number of times through sigTest */

		int delay = 1;      			 /* delay count in seconds */
		timer_t   timerId;                       /* id  for the posix timer */
    		struct    itimerspec   timeToSet;        /* time to be set */  
   	 	struct    timespec     timeValue;        /* timer expiration value */
    		struct    timespec     timeInterval;     /* timer period */
    		struct    sigaction    signalAction;     /* signal action handler struct */

    		/* Intialize sigaction struct */
    		signalAction.sa_handler = (VOIDFUNCPTR) sigHandler;
    		signalAction.sa_mask    = 0;
    		signalAction.sa_flags   = 0;

    		/* Initialize timer expiration value */
    		timeValue.tv_sec        = delay;
    		timeValue.tv_nsec       = 0;

    		/* Initialize timer period */
    		timeInterval.tv_sec     = delay;
    		timeInterval.tv_nsec    = 0;

    		/* Set the time to be set value */
    		timeToSet.it_value      = timeValue;
    		timeToSet.it_interval   = timeInterval;

    		/* Connect a signal handler routine to the SIGALRM siganl */
    		if (sigaction (SIGALRM, &signalAction, NULL) == ERROR)
        	{
        		perror ("posixTimerStart: Error in executing sigaction");
        		return (ERROR);
        	}
    
    		printf ("posixTimerStart: Unblock the demoTask in %d seconds\n", delay);
    
    		/* Allocate a timer */
    		if ((timer_create (CLOCK_REALTIME, NULL,&timerId)) == ERROR)
        	{
       			perror ("posixTimerStart: Error in allocating a timer");
        		return (ERROR);
        	}

    		/* Set the time  until the next expiration and arm the timer (POSIX) */
    		if (timer_settime (timerId, TIMER_RELATIVE, &timeToSet, NULL) == ERROR)
        	{
        		perror ("posixTimerStart: Error in setting time");
        		return (ERROR);
        	}
		FOREVER {
    			printf ("\nParent sigTest has just completed loop number %d\n",
			loopCount);
			taskDelay(sysClkRateGet() * 10);
    			loopCount++;
			if(loopCount>iteration)
			{
				 /* delete the previously created timer */
    				if (timer_delete (timerId) == ERROR)
        			{
        				perror ("posixTimerStart: Error in removing timer (POSIX)");
        				return (ERROR);
        			}
				break;
			}
		}
	}

