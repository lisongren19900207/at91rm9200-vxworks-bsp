/*-----------------------------EBI--------------------*/

#ifndef _AT91_EBI_DEFINE_HEADER
#define _AT91_EBI_DEFINE_HEADER

#ifndef _ASMLANGUAGE

typedef struct _EBI_S {
	AT91_REG	 EBI_CSA; 	/* Chip Select Assignment Register*/
	AT91_REG	 EBI_CFGR; 	/* Configuration Register*/
} EBI_S;

#endif

#define	EBI_BASE_ADDR	0xFFFFFF60

#define	EBI_CSA_OFFSET	(0)
#define	EBI_CFGR_OFFSET	(0x04)

#define	EBI_CSA_CS0A	0
#define	EBI_CSA_CS1A	(0x1<<1)
#define	EBI_CSA_CS3A	(0x0<<3)
#define	EBI_CSA_CS4A	(0x0<<4)

#define	EBI_CSA_VALUE	(EBI_CSA_CS0A|EBI_CSA_CS1A|EBI_CSA_CS3A|EBI_CSA_CS4A)

#endif /*_AT91_EBI_DEFINE_HEADER*/

