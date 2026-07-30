#include "stdint.h"
#include "UART/UART_Register.h"
#include "UART/UART_interface.h"
#include "UART/UART_config.h"

void (*ptr_fun[6])(uint8_t)={0};
uint32_t RCC_GetPCLK1(void) {

    return 16000000;
}

uint32_t RCC_GetPCLK2(void) {

    return 16000000;
}

void UART_Init(const USART_Config_t* config) {
    // Enable USART2 clock also pin definition
    USART_RegDef_t* USARTx = config->USART_Instance;
	uint64_t PCLK = 0; // Assume APB1 = 16MHz

	if (USARTx == USART1 || USARTx == USART6)  PCLK = RCC_GetPCLK2();                   // USART1 is on APB2
	else if (USARTx == USART2 || USARTx == USART3|| USARTx == UART4|| USARTx == UART5)  PCLK = RCC_GetPCLK1();              // USART2 is on APB1

    uint32_t USART_DIV =0;
    uint16_t DIV_Mantissa =0;
    uint16_t DIV_Fraction =0;
    if (config->Sampling == OVER_SAMPLING_DIV16) USART_DIV =((PCLK *1000) / (16*(config->BaudRate)));
    else USART_DIV =((PCLK *1000) / (8*(config->BaudRate)));
    DIV_Mantissa =(uint16_t) USART_DIV/1000;
    DIV_Fraction =(uint16_t) USART_DIV%1000;

    if (config->Sampling == OVER_SAMPLING_DIV16) DIV_Fraction =(uint16_t)(DIV_Fraction * 16 + 500)/1000;
    else DIV_Fraction= (uint16_t)(DIV_Fraction * 8 + 500)/1000;
    USARTx->BRR =0;
    USARTx->BRR |=((DIV_Mantissa & 0xFFF)<<4) |((DIV_Fraction &0xF)<<0);

    USARTx->CR2 &= ~(0x3 << 12);                          // Clear STOP bits
    USARTx->CR2 |= ((config->StopBits & 0x3) << 12);      // Set STOP bits


    USARTx->CR1 = 0;
    USARTx->CR1 |= config->Mode;

    USARTx->CR1 |= (config->DataBits << 12);

    if (config->Sampling == OVER_SAMPLING_DIV8)
        USARTx->CR1 |= (1 << 15);
    else
        USARTx->CR1 &= ~(1 << 15);


    USARTx->CR1 |= (config->Parity == USART_PARITY_EVEN) ? USART_CR1_PCE : 0;
    USARTx->CR1 |= (config->Parity == USART_PARITY_ODD) ? (USART_CR1_PCE | USART_CR1_PS) : 0;
    if(config->USART_Instance == USART1) ptr_fun[0]=config->ptr_callback;
    else if(config->USART_Instance == USART2) ptr_fun[1]=config->ptr_callback;
    else if(config->USART_Instance == USART3) ptr_fun[2]=config->ptr_callback;
    else if(config->USART_Instance == UART4) ptr_fun[3]=config->ptr_callback;
    else if(config->USART_Instance == UART5) ptr_fun[4]=config->ptr_callback;
    else if(config->USART_Instance == USART6) ptr_fun[5]=config->ptr_callback;
    USARTx->CR1 |= USART_CR1_UE;
}


void USART_SendData(USART_RegDef_t* USARTx, uint8_t* data, uint16_t len) {

    for (uint16_t i = 0; i < len; i++) {
    	uint32_t UART_time=TIME_OUT;
        while (!(USARTx->SR & USART_SR_TXE) && UART_time>0) UART_time--;
        if(UART_time==0)return ;
        USARTx->DR = data[i];
    }
    while (!(USARTx->SR & USART_SR_TC));
}

uint32_t USART_ReceiveData(USART_RegDef_t* USARTx, uint8_t* buffer, uint16_t len) {
	uint32_t local_u32Counter =0;
    for (uint16_t i = 0; i < len; i++) {
    	uint32_t UART_time=TIME_OUT;
        while (!(USARTx->SR & USART_SR_RXNE)&& UART_time>0) UART_time--;
        if(UART_time==0)
		{
		return local_u32Counter ;
		}
        buffer[i] = USARTx->DR;
        local_u32Counter++;
    }
    return local_u32Counter;
}
uint32_t USART_ReceiveData_word(USART_RegDef_t* USARTx, uint8_t* buffer, uint16_t len)
{
    uint32_t local_u32Counter = 0;
    uint32_t UART_time;

    // Wait for the FIRST byte, but bounded by a timeout instead of forever
    UART_time = 5000000;   // tune this - large enough to cover realistic wait
    while (!(USARTx->SR & USART_SR_RXNE) && UART_time > 0) UART_time--;
    if (UART_time == 0)
    {
        return 0;   // nothing received at all -> timeout, no message
    }
    buffer[local_u32Counter++] = USARTx->DR;

    // After that, use a short inter-byte timeout to know when the message ends
    for (; local_u32Counter < len; )
    {
        UART_time = 200000;   // must be much larger than before - tune experimentally
        while (!(USARTx->SR & USART_SR_RXNE) && UART_time > 0) UART_time--;
        if (UART_time == 0) break;   // no more bytes came in time -> end of message
        buffer[local_u32Counter++] = USARTx->DR;
    }

    buffer[local_u32Counter] = '\0';
    return local_u32Counter;
}
/*
uint32_t USART_ReceiveData_word(USART_RegDef_t* USARTx, uint8_t* buffer, uint16_t len)
{
    uint32_t local_u32Counter = 0;

    // Wait (long/indefinitely) for the first byte
    while (!(USARTx->SR & USART_SR_RXNE));

    buffer[local_u32Counter++] = USARTx->DR;

    // After that, use a short inter-byte timeout to know when the message ends
    for (; local_u32Counter < len; )
    {
        uint32_t UART_time = 1000; // much smaller, tune experimentally
        while (!(USARTx->SR & USART_SR_RXNE) && UART_time > 0) UART_time--;
        if (UART_time == 0) break; // no more bytes came in time -> end of message
        buffer[local_u32Counter++] = USARTx->DR;
    }
    return local_u32Counter;
}

*/
