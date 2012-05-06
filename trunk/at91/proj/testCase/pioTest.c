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

/* Port C */
#define PIOC_BASE		0xFFFFF800
#define PIOC_REG(x)	    *(volatile UINT32 *)(PIOC_BASE + (x))

/* Registers */
#define PIO_PER 	0x00	/* PIO Enable Register              */ 
#define PIO_PDR 	0x04	/* PIO Disable Register             */ 
#define PIO_PSR 	0x08	/* PIO Status Register              */ 
#define PIO_OER 	0x10	/* Output Enable Register           */ 
#define PIO_ODR 	0x14	/* Output Disable Registerr         */ 
#define PIO_OSR 	0x18	/* Output Status Register           */ 
#define PIO_IFER 	0x20	/* Input Filter Enable Register     */ 
#define PIO_IFDR 	0x24	/* Input Filter Disable Register    */ 
#define PIO_IFSR 	0x28	/* Input Filter Status Register     */ 
#define PIO_SODR 	0x30	/* Set Output Data Register         */ 
#define PIO_CODR 	0x34	/* Clear Output Data Register       */ 
#define PIO_ODSR 	0x38	/* Output Data Status Register      */ 
#define PIO_PDSR 	0x3c	/* Pin Data Status Register         */ 
#define PIO_IER 	0x40	/* Interrupt Enable Register        */ 
#define PIO_IDR 	0x44	/* Interrupt Disable Register       */ 
#define PIO_IMR 	0x48	/* Interrupt Mask Register          */ 
#define PIO_ISR 	0x4c	/* Interrupt Status Register        */ 
#define PIO_MDER 	0x50	/* Multi-driver Enable Register     */ 
#define PIO_MDDR 	0x54	/* Multi-driver Disable Register    */ 
#define PIO_MDSR 	0x58	/* Multi-driver Status Register     */ 
#define PIO_PUDR 	0x60	/* Pull-up Disable Register         */ 
#define PIO_PUER 	0x64	/* Pull-up Enable Register          */ 
#define PIO_PUSR 	0x68	/* Pad Pull-up Status Register      */ 
#define PIO_ASR 	0x70	/* Select A Register                */ 
#define PIO_BSR 	0x74	/* Select B Register                */ 
#define PIO_ABSR 	0x78	/* AB Select Status Register        */ 
#define PIO_OWER 	0xA0	/* Output Write Enable Register     */ 
#define PIO_OWDR 	0xA4	/* Output Write Disable Register    */ 
#define PIO_OWSR 	0xA8	/* Output Write Status Register     */ 

#define AT91C_PIO_PC14       ((unsigned int) 1 << 14) /* Pin Controlled by PC14*/
#define AT91C_PIO_PC15       ((unsigned int) 1 << 15) /* Pin Controlled by PC15*/

const int led_mask[] = { AT91C_PIO_PC14, AT91C_PIO_PC15};
char led0_on = 1;

WDOG_ID	wdId;			/* watchdog ID */

STATUS tickInt(int parameter)
{
	led0_on = !led0_on;
	led_ctl(0,led0_on);
	
	logMsg("Watchdog timer just expired,restart watchdog.\n",0,0,0,0,0,0);
	if(wdStart (wdId, sysClkRateGet() * 5, (FUNCPTR)tickInt, 1) == ERROR)
		return (ERROR);
}
void init_led(void)
{
 	/* Configure the PIO Lines corresponding to USER LED1 USER LED2 as Outputs*/
  	PIOC_REG(PIO_OER) = AT91C_PIO_PC14 | AT91C_PIO_PC15;

 	/* Clear the LED's. On the Board we must apply a "1" to turn off LEDs */
  	PIOC_REG(PIO_SODR) = AT91C_PIO_PC14 | AT91C_PIO_PC15;
	
}

void led_ctl(int which, char on)
{
	if(which < 2)
	{
		if(which == 0)
		{
			if(on)
			{
				PIOC_REG(PIO_SODR) = led_mask[which];
			}
			else
			{
				PIOC_REG(PIO_CODR) = led_mask[which];
			}
		}
		if(which == 1)
		{
			if(on)
			{
				PIOC_REG(PIO_SODR) = led_mask[which];
			}
			else
			{
				PIOC_REG(PIO_CODR) = led_mask[which];
			}
		}
	}
}

STATUS pioTest()
{
	init_led();
	led_ctl(0,led0_on);
	/* create watchdog timer */	
	if((wdId = wdCreate ()) == NULL)
		return (ERROR);
	printf("wdId=%d\n",wdId);
	
	if(wdStart (wdId, sysClkRateGet() * 5, (FUNCPTR)tickInt, 1) == ERROR)
		return (ERROR);
}
