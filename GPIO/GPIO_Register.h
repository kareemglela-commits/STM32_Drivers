#ifndef _GPIO_REGISTER_H
#define _GPIO_REGISTER_H

#define GPIOA_BASE_ADDRESS 0x40020000
#define GPIOB_BASE_ADDRESS 0x40020400
#define GPIOC_BASE_ADDRESS 0x40020800
#define GPIOD_BASE_ADDRESS 0x40020C00
#define GPIOE_BASE_ADDRESS 0x40021000
#define GPIOF_BASE_ADDRESS 0x40021400
#define GPIOG_BASE_ADDRESS 0x40021800
#define GPIOH_BASE_ADDRESS 0x40021C00

typedef struct {
	volatile uint32_t GPIO_MODER;
	volatile uint32_t GPIO_OTYPER;
	volatile uint32_t GPIO_OSPEEDER;
	volatile uint32_t GPIO_PUPDR;
	volatile uint32_t GPIO_IDR;
	volatile uint32_t GPIO_ODR;
	volatile uint32_t GPIO_BSRR;
	volatile uint32_t GPIO_LCKR;
	volatile uint32_t GPIO_AFR[2];
}GPIO_RegDef_t;


#define GPIOA ((GPIO_RegDef_t*)(GPIOA_BASE_ADDRESS))
#define GPIOB ((GPIO_RegDef_t*)(GPIOB_BASE_ADDRESS))
#define GPIOC ((GPIO_RegDef_t*)(GPIOC_BASE_ADDRESS))
#define GPIOD ((GPIO_RegDef_t*)(GPIOD_BASE_ADDRESS))
#define GPIOE ((GPIO_RegDef_t*)(GPIOE_BASE_ADDRESS))
#define GPIOF ((GPIO_RegDef_t*)(GPIOF_BASE_ADDRESS))

#endif /*_RCC_REGISTER_H*/