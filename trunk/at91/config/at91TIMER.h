
#ifndef _AT91_TIMER_DEFINE_HEADER
#define _AT91_TIMER_DEFINE_HEADER

/*-------------------------------System Timer------------------------------*/
#define SYSTIMER_BASE_ADDR	0xFFFFFD00
#define	TIMER0_BASE_ADDR	0xFFFA0000
#define	TIMER1_BASE_ADDR	0xFFFA0040
#define	TIMER2_BASE_ADDR	0xFFFA0080

#define	TIMER3_BASE_ADDR	0xFFFA4000
#define	TIMER4_BASE_ADDR	0xFFFA4040
#define	TIMER5_BASE_ADDR	0xFFFA4080
#define TC_BCR_OFFSET	0xc0
#define TC_BMR_OFFSET	0xc4

#define TC0_TC0XC0S	(0x2)
#define TC0_TC0XC1S	((0x2)<<2)
#define TC0_TC0XC2S	((0x2)<<4)
#define TC0_BMR_VALUE (TC0_TC0XC0S | TC0_TC0XC1S | TC0_TC0XC2S  )



#ifndef	_ASMLANGUAGE
typedef struct _SYSTIMER_S
{	
	AT91_REG	 ST_CR; 	/* Control Register			*/
	AT91_REG	 ST_PIMR; 	/* Period Interval Mode Register        */
	AT91_REG	 ST_WDMR; 	/* Watchdog Mode Register               */
	AT91_REG	 ST_RTMR; 	/* Real-time Mode Register              */
	AT91_REG	 ST_SR; 	/* Status Register                      */
	AT91_REG	 ST_IER; 	/* Interrupt Enable Register            */
	AT91_REG	 ST_IDR; 	/* Interrupt Disable Register           */
	AT91_REG	 ST_IMR; 	/* Interrupt Mask Register              */
	AT91_REG	 ST_RTAR; 	/* Real-time Alarm Register             */
	AT91_REG	 ST_CRTR; 	/* Current Real-time Register		*/
}SYSTIMER_S;


typedef struct _TimerCounter
{
	AT91_REG	 TC_CCR; 	/* Channel Control Register	*/
	AT91_REG	 TC_CMR; 	/* Channel Mode Register*/
	AT91_REG	 Reserved0[2]; 	/* */
	AT91_REG	 TC_CV; 	/* Counter Value*/
	AT91_REG	 TC_RA; 	/* Register A*/
	AT91_REG	 TC_RB; 	/* Register B*/
	AT91_REG	 TC_RC; 	/* Register C*/
	AT91_REG	 TC_SR; 	/* Status Register*/
	AT91_REG	 TC_IER; 	/* Interrupt Enable Register*/
	AT91_REG	 TC_IDR; 	/* Interrupt Disable Register*/
	AT91_REG	 TC_IMR; 	/* Interrupt Mask Register*/
} TimerCounter;
#endif

#define AT91_TIMER_CLOCK_1 (MASTER_CLK/2)
#define AT91_TIMER_CLOCK_2 (MASTER_CLK/8)
#define AT91_TIMER_CLOCK_3 (MASTER_CLK/32)
#define AT91_TIMER_CLOCK_4 (MASTER_CLK/128)

/* timer 0 is used as main clock source of the vxworks */
#define TIMER0_CLOCK		AT91_TIMER_CLOCK_4
#define TIMER0_TCMR_TCCLKS	(0x3)

#define TIMER0_TCMR_CLKI	(0<<3)
#define TIMER0_TCMR_BURST	(0<<4)
#define TIMER0_TCMR_CPCSTOP	(0<<6)
#define TIMER0_TCMR_CPCDIS	(0<<7)
#define TIMER0_TCMR_EECTEDG	(0<<8)
#define TIMER0_TCMR_EEVT	(0<<10)
#define TIMER0_TCMR_ENETRG	(0<<12)
#define TIMER0_TCMR_WAVESEL	((0x2)<<13)	/*with RC compare*/
#define TIMER0_TCMR_WAVE	(1<<15)
#define TIMER0_TCMR_ACPA	(0<<16)
#define TIMER0_TCMR_ACPC	(0<<18)
#define TIMER0_TCMR_AEEVT	(0<<20)
#define TIMER0_TCMR_ASWTRG	(0<<22)
#define TIMER0_TCMR_BCPB	(0<<24)
#define TIMER0_TCMR_BCPC	(0<<26)
#define TIMER0_TCMR_BEEVT	(0<<28)
#define TIMER0_TCMR_BSWTRG	(0<<30)

#define TIMER0_TCMR_VALUE \
	( TIMER0_TCMR_TCCLKS | TIMER0_TCMR_CLKI | TIMER0_TCMR_BURST | TIMER0_TCMR_CPCSTOP \
		| TIMER0_TCMR_CPCDIS | TIMER0_TCMR_EECTEDG | TIMER0_TCMR_EEVT | TIMER0_TCMR_ENETRG \
		| TIMER0_TCMR_WAVESEL| TIMER0_TCMR_WAVE | TIMER0_TCMR_ACPA|TIMER0_TCMR_ACPC \
		| TIMER0_TCMR_AEEVT | TIMER0_TCMR_ASWTRG | TIMER0_TCMR_BCPB | TIMER0_TCMR_BCPC \
		| TIMER0_TCMR_BEEVT | TIMER0_TCMR_BSWTRG )


#define TIMER0_CCR_CLKEN	(1)
#define TIMER0_CCR_CLKDIS	(1<<1)
#define TIMER0_CCR_SWTRG	(1<<2)
#define TIMER0_CCR_VALUE	( TIMER0_CCR_CLKEN | TIMER0_CCR_SWTRG )

/*
 *	RC contain the counter value of system clock
 */
/*#define TIMER0_RC_VALUE		( TIMER0_CLOCK/sysClkTicksPerSecond )*/

/*
 *	Time 0 clock only use  RC compare result as interrupt source
 */
#define TIMER0_IER_VALUE	(1<<4)
#define TIMER0_IDR_VALUE	((~TIMER0_IER_VALUE)&0xff)



/*
 *	Timer 1 used as AUX clock
 */

#define TIMER1_CLOCK		AT91_TIMER_CLOCK_4
#define TIMER1_TCMR_TCCLKS	(0x3)
#define TIMER1_TCMR_CLKI	(0<<3)
#define TIMER1_TCMR_BURST	(0<<4)
#define TIMER1_TCMR_CPCSTOP	(0<<6)
#define TIMER1_TCMR_CPCDIS	(0<<7)
#define TIMER1_TCMR_EECTEDG	(0<<8)
#define TIMER1_TCMR_EEVT	(0<<10)
#define TIMER1_TCMR_ENETRG	(0<<12)
#define TIMER1_TCMR_WAVESEL	((0x2)<<13)	/*with RC compare*/
#define TIMER1_TCMR_WAVE	(1<<15)
#define TIMER1_TCMR_ACPA	(0<<16)
#define TIMER1_TCMR_ACPC	(0<<18)
#define TIMER1_TCMR_AEEVT	(0<<20)
#define TIMER1_TCMR_ASWTRG	(0<<22)
#define TIMER1_TCMR_BCPB	(0<<24)
#define TIMER1_TCMR_BCPC	(0<<26)
#define TIMER1_TCMR_BEEVT	(0<<28)
#define TIMER1_TCMR_BSWTRG	(0<<30)

#define TIMER1_TCMR_VALUE \
	( TIMER1_TCMR_TCCLKS | TIMER1_TCMR_CLKI | TIMER1_TCMR_BURST | TIMER1_TCMR_CPCSTOP \
	| TIMER1_TCMR_CPCDIS | TIMER1_TCMR_EECTEDG | TIMER1_TCMR_EEVT | TIMER1_TCMR_ENETRG \
	| TIMER1_TCMR_WAVESEL| TIMER1_TCMR_WAVE | TIMER1_TCMR_ACPA|TIMER1_TCMR_ACPC \
	| TIMER1_TCMR_AEEVT | TIMER1_TCMR_ASWTRG | TIMER1_TCMR_BCPB | TIMER1_TCMR_BCPC \
 	| TIMER1_TCMR_BEEVT | TIMER1_TCMR_BSWTRG )


#define TIMER1_CCR_CLKEN	(1)
#define TIMER1_CCR_CLKDIS	(1<<1)
#define TIMER1_CCR_SWTRG	(1<<2)
#define TIMER1_CCR_VALUE	( TIMER1_CCR_CLKEN | TIMER1_CCR_SWTRG )
 

/*
 *	Time 1 clock only use  RC compare result as interrupt source
 */
#define TIMER1_IER_VALUE	(1<<4)
#define TIMER1_IDR_VALUE	((~TIMER1_IER_VALUE)&0xff)

#define TIMER1_RC_VALUE       (1<<4)

/*
 *	Timer 1 used as AUX clock
 */

#define TIMER2_CLOCK		AT91_TIMER_CLOCK_4
#define TIMER2_TCMR_TCCLKS	(0x3)
#define TIMER2_TCMR_CLKI	(0<<3)
#define TIMER2_TCMR_BURST	(0<<4)
#define TIMER2_TCMR_CPCSTOP	(0<<6)
#define TIMER2_TCMR_CPCDIS	(0<<7)
#define TIMER2_TCMR_EECTEDG	(0<<8)
#define TIMER2_TCMR_EEVT	(0<<10)
#define TIMER2_TCMR_ENETRG	(0<<12)
#define TIMER2_TCMR_WAVESEL	((0x2)<<13)	/*with RC compare*/
#define TIMER2_TCMR_WAVE	(1<<15)
#define TIMER2_TCMR_ACPA	(0<<16)
#define TIMER2_TCMR_ACPC	(0<<18)
#define TIMER2_TCMR_AEEVT	(0<<20)
#define TIMER2_TCMR_ASWTRG	(0<<22)
#define TIMER2_TCMR_BCPB	(0<<24)
#define TIMER2_TCMR_BCPC	(0<<26)
#define TIMER2_TCMR_BEEVT	(0<<28)
#define TIMER2_TCMR_BSWTRG	(0<<30)

#define TIMER2_TCMR_VALUE \
	( TIMER2_TCMR_TCCLKS | TIMER2_TCMR_CLKI | TIMER2_TCMR_BURST | TIMER2_TCMR_CPCSTOP \
	| TIMER2_TCMR_CPCDIS | TIMER2_TCMR_EECTEDG | TIMER2_TCMR_EEVT | TIMER2_TCMR_ENETRG \
	| TIMER2_TCMR_WAVESEL| TIMER2_TCMR_WAVE | TIMER2_TCMR_ACPA|TIMER2_TCMR_ACPC \
	| TIMER2_TCMR_AEEVT | TIMER2_TCMR_ASWTRG | TIMER2_TCMR_BCPB | TIMER2_TCMR_BCPC \
 	| TIMER2_TCMR_BEEVT | TIMER2_TCMR_BSWTRG )


#define TIMER2_CCR_CLKEN	(1)
#define TIMER2_CCR_CLKDIS	(1<<1)
#define TIMER2_CCR_SWTRG	(1<<2)
#define TIMER2_CCR_VALUE	( TIMER2_CCR_CLKEN | TIMER2_CCR_SWTRG )
 

/*
 *	Time 1 clock only use  RC compare result as interrupt source
 */
#define TIMER2_IER_VALUE	(1<<4)
#define TIMER2_IDR_VALUE	((~TIMER2_IER_VALUE)&0xff)

#define TIMER2_RC_VALUE       (1<<4)



/* system clk tick max and min, min about 7Hz for 60M Master clock
	12Hz for 100MHz clock
*/
#define SYS_CLK_RATE_MIN (SLOW_RATE_CLK/16384)
#define SYS_CLK_RATE_MAX 1000
#define DEFAULT_SYS_CLK_RATE	60

#define AUX_CLK_RATE_MIN (TIMER0_CLOCK/65530)
#define AUX_CLK_RATE_MAX 1000


#define ST_CR_WDRST		(1<<0)
#define ST_WDMR_RSTEN	(1<<16)
#define ST_WDMR_EXTEN	(1<<17)
#define ST_WDMR_WDV		(0xFFFF)

#define ST_SR_PITS		(1<<0)
#define ST_SR_WDODF		(1<<1)
#define ST_SR_RTTINC	(1<<2)
#define ST_SR_ALARMS	(1<<3)

#define ST_IE_DR_PITS		(1<<0)
#define ST_IE_DR_WDOVF		(1<<1)
#define ST_IE_DR_RTTINC		(1<<2)
#define ST_IE_DR_ALARMS		(1<<3)

#define ST_RTMT_RT_PRESCALE	1
#define	ST_RTT_MAX_COUNT_VALUE	(0x100000)


#endif /*_AT91_TIMER_DEFINE_HEADER*/

