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

#include "at91rm9200.h"

#define AT91C_PC10_NCS4_CFCS      ((unsigned int) 1 << 10)
#define AT91C_PC9_A25_CFRNW       ((unsigned int) 1 << 9)
#define AT91C_PC12_NCS6_CFCE2	  ((unsigned int) 1 << 12)
#define AT91C_PC11_NCS5_CFCE1	  ((unsigned int) 1 << 11)

#define BASE_ADDR		(0x70000000)

#define OFFSET_ADC1_CH	(0x40)
#define OFFSET_ADC1_CS	(0x60)	/* AD偏移地址 */

#define ADC1			(BASE_ADDR + OFFSET_ADC1_CS)
#define ADC1_CH			(BASE_ADDR + OFFSET_ADC1_CH)

#define CHANTOTAL_ADC 16

#define  TASK_PRI         60    /* Priority of spawned tasks */
#define  TASK_STACK_SIZE  10000 /* stack size for spawned tasks */

short inpw(unsigned int addr)
{
	return *((volatile unsigned short* )addr);
}

void outpw(unsigned int addr, short newval)
{
	*((volatile unsigned short* )addr) = newval;
}

void initSMC(void)
{
	unsigned int periphAEnable=((unsigned int) AT91C_PC10_NCS4_CFCS) |
		((unsigned int) AT91C_PC9_A25_CFRNW) |
		((unsigned int) AT91C_PC12_NCS6_CFCE2) |
		((unsigned int) AT91C_PC11_NCS5_CFCE1);
	unsigned int periphBEnable=0; 		
	
	PIOC_REG(PIO_ASR) = periphAEnable;
	PIOC_REG(PIO_BSR) = periphBEnable;
	PIOC_REG(PIO_PDR)=(periphAEnable | periphBEnable);

	SMC_REG(SMC_CSR6) = ((SMC2_NWS & 0x7) | SMC2_WSEN | (SMC2_TDF & 0x100)
		 | SMC2_BAT | SMC2_DBW_16 | (SMC2_RWHOLD & (0x1 << 28)));
}
void readADCTask(void)
{	
	int i;
	int a;
	unsigned short data;
	for(i = 0; i < CHANTOTAL_ADC; i++)
	{
		/*选择通道*/
		outpw((ADC1_CH + i * 2), 0x0);
		/**((volatile unsigned short *)(ADC1_CH + i * 2)) = 0x0;*/
		taskDelay(20);/*20ms*/
			
		/*启动采样*/
		outpw(ADC1, 0x0);
			
		/*等待AD转换好*/
		taskDelay(20);/*20ms*/
			
		/*读取*/
		data = inpw(ADC1);
		a=5 * data * 1000/0xFFFF;
		printf("-D- ADC ch%d: 0x%X %d.%dv\n\r",i,data,a/1000,a-a/1000*1000);
	}
}

STATUS adcTest()
{
	initSMC();

	if (taskSpawn ("readADCTask", TASK_PRI, 0, TASK_STACK_SIZE, 
                      (FUNCPTR) readADCTask, 0, 0, 0, 0, 0, 0, 0, 0, 
                       0, 0) == ERROR)
    	{
        	perror ("uartTest: Error in spawning uartTest task");
        	return (ERROR);
    	} 
}
