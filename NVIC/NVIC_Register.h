#ifndef NVIC_REGISTER_H
#define NVIC_REGISTER_H

				   /*************  Nested Vectored Interrupt Control *********/
#define NVIC_BASE_ADDRESS             0xE000E100U

typedef struct
{
	volatile uint32_t NVIC_ISER[8];
	uint32_t		  RESERVED0[24];
	volatile uint32_t NVIC_ICER[8];
	uint32_t 		  RESERVED1[24];
	volatile uint32_t NVIC_ISPR[8];
	uint32_t 		  RESERVED2[24];
	volatile uint32_t NVIC_ICPR[8];
	uint32_t 		  RESERVED3[24];
	volatile uint32_t NVIC_IABR[8];
	uint32_t 		  RESERVED4[56];
	volatile uint32_t NVIC_IPR [60];
	uint32_t 		  RESERVED5[644];
	volatile uint32_t STIR;
}NVIC_RegDef_t;

#define NVIC			 ((NVIC_RegDef_t*)(NVIC_BASE_ADDRESS))




#endif