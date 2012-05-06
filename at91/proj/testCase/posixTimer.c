/* posixTimer.c - posix timer demo */

/* Copyright 1984-1997 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,07nov97,mm   added modification history and copyright
*/

/* includes */

#include "vxWorks.h"
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "taskLib.h"

/* defines */

#define LONGTIME 10000

timer_t timerId; 
struct itimerspec time_value;

void alarmHandle (timer_t ti, int arg)
{
printf ("Timer expired with the message %s!\n", (char *) arg);
}

void setup ()
{
/* create a new timer and put its id in timerId */
timer_create (CLOCK_REALTIME, NULL, &timerId);

/* When the timer goes off we want to call alarmHandle */
timer_connect (timerId, (VOIDFUNCPTR) alarmHandle, (int) "Hi!");

/* initialise time-value */
bzero ((char *) &time_value, sizeof (struct itimerspec));

/* Timer goes off after .5 secs */
time_value.it_value.tv_nsec = 500000000; 


/* arm timer */
timer_settime (timerId, 0, &time_value, NULL);

if (taskDelay (LONGTIME) != OK)
   printf ("Task woken up by alarm!\n");

/* 
* When the timer goes off it sends a signal to the task that armed it.
* The signal handler alarmHandle will be called in the context of this task.
* As a result of the signal taskDelay will return with ERROR 
*/
   
else
   timer_cancel (timerId);

/* Clean up any resources used by the timer */
   timer_delete (timerId);
}

