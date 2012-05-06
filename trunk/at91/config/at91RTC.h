
#ifndef _AT91_RTC_DEFINE_HEADER
#define _AT91_RTC_DEFINE_HEADER



#define RTC_BASE_ADDR	0xFFFFFE00
#ifndef _ASMLANGUAGE
typedef struct _RTC_S {
	AT91_REG	 RTC_CR; 		/* Control Register              	*/
	AT91_REG	 RTC_MR; 		/* Mode Register                    */
	AT91_REG	 RTC_TIMR; 		/* Time Register                    */
	AT91_REG	 RTC_CALR; 		/* Calendar Register                */
	AT91_REG	 RTC_TIMALR; 	/* Time Alarm Register              */
	AT91_REG	 RTC_CALALR; 	/* Calendar Alarm Register          */
	AT91_REG	 RTC_SR; 		/* Status Register                  */
	AT91_REG	 RTC_SCCR; 		/* Status Clear Command Register    */
	AT91_REG	 RTC_IER; 		/* Interrupt Enable Register        */
	AT91_REG	 RTC_IDR; 		/* Interrupt Disable Register       */
	AT91_REG	 RTC_IMR; 		/* Interrupt Mask Register          */
	AT91_REG	 RTC_VER; 		/* Valid Entry Register             */
} RTC_S;
#endif


#endif /*_AT91_RTC_DEFINE_HEADER*/

