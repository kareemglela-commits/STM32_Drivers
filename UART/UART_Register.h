#ifndef _UART_REGISTER_H
#define _UART_REGISTER_H

#define USART1_BASE_ADDRESS         0x40011000U
#define USART2_BASE_ADDRESS         0x40004400U
#define USART3_BASE_ADDRESS         0x40004800U
#define UART4_BASE_ADDRESS          0x40004C00U
#define UART5_BASE_ADDRESS          0x40005000U
#define USART6_BASE_ADDRESS         0x40011400U
typedef struct {
	volatile uint32_t SR  ;
	volatile uint32_t DR  ;
	volatile uint32_t BRR ;
	volatile uint32_t CR1 ;
	volatile uint32_t CR2 ;
	volatile uint32_t CR3 ;
	volatile uint32_t GTPR;
}USART_RegDef_t;


#define USART1 ((USART_RegDef_t*)(USART1_BASE_ADDRESS))
#define USART2 ((USART_RegDef_t*)(USART2_BASE_ADDRESS))
#define USART3 ((USART_RegDef_t*)(USART3_BASE_ADDRESS))
#define UART4  ((USART_RegDef_t*)(UART4_BASE_ADDRESS))
#define UART5  ((USART_RegDef_t*)(UART5_BASE_ADDRESS))
#define USART6 ((USART_RegDef_t*)(USART6_BASE_ADDRESS))

#endif /*_RCC_REGISTER_H*/
