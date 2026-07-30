#ifndef _RCC_REGISTER_H
#define _RCC_REGISTER_H

#define RCC_BASE_ADDRESS 0x40023800

typedef struct {
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_PLLCFGR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_AHB1RSTR;
	volatile uint32_t RCC_AHB2RSTR;
	volatile uint32_t RCC_AHB3RSTR;
	volatile uint32_t Reserved_0;
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_APB2RSTR;
	volatile uint32_t Reserved_1[2];
	
	volatile uint32_t RCC_AHB1ENR;
	volatile uint32_t RCC_AHB2ENR;
	volatile uint32_t RCC_AHB3ENR;
	
	volatile uint32_t Reserved_2;
	
	volatile uint32_t RCC_APB1ENR;
	volatile uint32_t RCC_APB2ENR;
	
	volatile uint32_t Reserved_3[2];
	
	volatile	uint32_t RCC_AHB1LPENR;
	volatile	uint32_t RCC_AHB2LPENR;
	volatile	uint32_t RCC_AHB3LPENR;
	volatile	uint32_t Reserved_4;
		
	volatile	uint32_t RCC_APB1LPENR;
	volatile	uint32_t RCC_APB2LPENR;
	
	volatile	uint32_t Reserved_5[2];
	volatile	uint32_t RCC_BDCR;
	volatile	uint32_t RCC_CSR;
	volatile	uint32_t Reserved_6[2];
	volatile	uint32_t RCC_SSCGR;
	volatile	uint32_t RCC_PLLI2SCFGR;
	volatile	uint32_t RCC_PLLSAICFGR;
	volatile	uint32_t RCC_DCKCFGR;
	volatile	uint32_t RCC_CKGATENR;
	volatile	uint32_t RCC_DCKCFGR2;
}RCC_RegDef_t;


#define RCC ((RCC_RegDef_t*)(RCC_BASE_ADDRESS))

#endif /*_RCC_REGISTER_H*/