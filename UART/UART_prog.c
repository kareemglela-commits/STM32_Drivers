#include "stdint.h"
#include "stdio.h"
#include "UART/UART_Register.h"
#include "UART/UART_interface.h"
#include "UART/UART_config.h"


#define USART_TX_BUFFER_SIZE 1024
#define USART_RX_BUFFER_SIZE 128
#define MAX_USART_COUNT      3  // Adjust for actual usage (e.g., USART1/2/3)
typedef struct {
    USART_RegDef_t* USARTx;

    volatile uint8_t TxBuffer[USART_TX_BUFFER_SIZE];
    volatile uint16_t TxHead;
    volatile uint16_t TxTail;
    volatile uint8_t TxBusy;

    volatile uint8_t RxBuffer[USART_RX_BUFFER_SIZE];
    volatile uint16_t RxHead;
    volatile uint16_t RxTail;
} USART_Handler_t;

static USART_Handler_t USART_Handlers[MAX_USART_COUNT];
static USART_Handler_t* GetUSARTHandler(USART_RegDef_t* USARTx) {
    for (int i = 0; i < MAX_USART_COUNT; i++) {
        if (USART_Handlers[i].USARTx == USARTx)
            return &USART_Handlers[i];
    }
    return NULL;
}


void (*ptr_fun[6])(uint8_t)={0};


void UART_Init(const USART_Config_t* config) {
    // Enable USART2 clock also pin definition
    USART_RegDef_t* USARTx = config->USART_Instance;
	uint64_t PCLK = 0; // Assume APB1 = 16MHz

	if (USARTx == USART1 || USARTx == USART6)  PCLK = 16000000;                   // USART1 is on APB2
	else if (USARTx == USART2 || USARTx == USART3|| USARTx == UART4|| USARTx == UART5)  PCLK = 16000000;              // USART2 is on APB1

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
    UART_time = 500000;   // tune this - large enough to cover realistic wait
    while (!(USARTx->SR & USART_SR_RXNE) && UART_time > 0) UART_time--;
    if (UART_time == 0)
    {
        return local_u32Counter;   // nothing received at all -> timeout, no message
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

void UART_Init_IT(const USART_Config_t* config) {
	UART_Init(config);
	USART_RegDef_t* USARTx = config->USART_Instance;
	USARTx->CR1 |= USART_RXNE_INTERRUPT_ENABLE;
	USARTx->CR1 |= USART_TXE_INTERRUPT_ENABLE;
}
void USART_RegisterHandler(USART_RegDef_t* USARTx)// before Initalization ( Enable also the Vector NVIC )
{
    for (int i = 0; i < MAX_USART_COUNT; i++) {
        if (USART_Handlers[i].USARTx == NULL) {
            USART_Handlers[i].USARTx = USARTx;
            USART_Handlers[i].TxHead = USART_Handlers[i].TxTail = 0;
            USART_Handlers[i].RxHead = USART_Handlers[i].RxTail = 0;
            USART_Handlers[i].TxBusy = 0;
            return;
        }
    }
}

void USART_SendData_IT(USART_RegDef_t* USARTx, uint8_t* data, uint16_t len) {
    USART_Handler_t* handler = GetUSARTHandler(USARTx);
    if (!handler || len == 0) return;

    for (uint16_t i = 0; i < len; i++) {
        uint16_t nextHead = (handler->TxHead + 1) % USART_TX_BUFFER_SIZE;
        if (nextHead == handler->TxTail)
            return; // Buffer full, drop or wait
        handler->TxBuffer[handler->TxHead] = data[i];
        handler->TxHead = nextHead;
    }

    // If not already transmitting, kick off the TXE interrupt
    if (!handler->TxBusy) {
        handler->TxBusy = 1;
        USARTx->CR1 |= USART_TXE_INTERRUPT_ENABLE;
    }
}

void USART_Read_IT(USART_RegDef_t* USARTx, uint8_t* data) {
    USART_Handler_t* handler = GetUSARTHandler(USARTx);
    if (!handler || handler->RxHead == handler->RxTail)
        return ;

    *data = handler->RxBuffer[handler->RxTail];
    handler->RxTail = (handler->RxTail + 1) % USART_RX_BUFFER_SIZE;
    return ;
}

void USART2_IRQHandler(void) // USED  for  Transmit Data Register Empty interrupt
{
    USART_Handler_t* handler = GetUSARTHandler(USART2);
    if (!handler) return;

    // Transmit Data Register Empty
    if ((USART2->SR & USART_SR_TXE) && (USART2->CR1 & USART_TXE_INTERRUPT_ENABLE)) {
        if (handler->TxTail != handler->TxHead) {
            USART2->DR = handler->TxBuffer[handler->TxTail];
            handler->TxTail = (handler->TxTail + 1) % USART_TX_BUFFER_SIZE;
        } else {
            // Buffer empty, disable TXE interrupt
            USART2->CR1 &= ~USART_TXE_INTERRUPT_ENABLE;
            handler->TxBusy = 0;
        }
    }
    // ... handle RXNE, etc.
}
/*
void USART2_IRQHandler(void) {
    USART_Handler_t* handler = GetUSARTHandler(USART2);
    if (!handler) return;

    // TXE: Transmit data register empty
    if ((USART2->SR & USART_SR_TXE) && (USART2->CR1 & USART_TXE_INTERRUPT_ENABLE)) {
        if (handler->TxHead != handler->TxTail) {
            USART2->DR = handler->TxBuffer[handler->TxTail];
            handler->TxTail = (handler->TxTail + 1) % USART_TX_BUFFER_SIZE;
        } else {
            USART2->CR1 &= ~USART_TXE_INTERRUPT_ENABLE; // Disable TXE interrupt
            handler->TxBusy = 0;
        }
    }

    // RXNE: Receive data register not empty
    if (USART2->SR & USART_SR_RXNE) {
        uint16_t nextHead = (handler->RxHead + 1) % USART_RX_BUFFER_SIZE;
        if (nextHead != handler->RxTail) { // Avoid overflow
        	uint8_t received = USART2->DR;
        	handler->RxBuffer[handler->RxHead] = received;
        	handler->RxHead = nextHead;

        	if (ptr_fun[1]) {
        	    ptr_fun[1](received);
        	}

        } else {
            volatile uint8_t dummy = USART2->DR; // Drop data
        }
    }
}
*/
void USART3_IRQHandler(void) {
    USART_Handler_t* handler = GetUSARTHandler(USART3);
    if (!handler) return;

    // TXE: Transmit data register empty
    if ((USART3->SR & USART_SR_TXE) && (USART3->CR1 & USART_TXE_INTERRUPT_ENABLE)) {
        if (handler->TxHead != handler->TxTail) {
            USART3->DR = handler->TxBuffer[handler->TxTail];
            handler->TxTail = (handler->TxTail + 1) % USART_TX_BUFFER_SIZE;
        } else {
            USART3->CR1 &= ~USART_TXE_INTERRUPT_ENABLE; // Disable TXE interrupt
            handler->TxBusy = 0;
        }
    }

    // RXNE: Receive data register not empty
    if (USART3->SR & USART_SR_RXNE) {
        uint16_t nextHead = (handler->RxHead + 1) % USART_RX_BUFFER_SIZE;
        if (nextHead != handler->RxTail) { // Avoid overflow
        	uint8_t received = USART3->DR;
        	        	handler->RxBuffer[handler->RxHead] = received;
        	        	handler->RxHead = nextHead;

        	        	if (ptr_fun[2]) {
        	        	    ptr_fun[2](received);
        	        	}
        } else {
            volatile uint8_t dummy = USART3->DR; // Drop data
        }
    }
}

void USART1_IRQHandler(void) {
    USART_Handler_t* handler = GetUSARTHandler(USART1);
    if (!handler) return;

    // TXE: Transmit data register empty
    if ((USART1->SR & USART_SR_TXE) && (USART1->CR1 & USART_TXE_INTERRUPT_ENABLE)) {
        if (handler->TxHead != handler->TxTail) {
            USART1->DR = handler->TxBuffer[handler->TxTail];
            handler->TxTail = (handler->TxTail + 1) % USART_TX_BUFFER_SIZE;
        } else {
            USART1->CR1 &= ~USART_TXE_INTERRUPT_ENABLE; // Disable TXE interrupt
            handler->TxBusy = 0;
        }
    }

    // RXNE: Receive data register not empty
    if (USART1->SR & USART_SR_RXNE) {
        uint16_t nextHead = (handler->RxHead + 1) % USART_RX_BUFFER_SIZE;
        if (nextHead != handler->RxTail) { // Avoid overflow
        	uint8_t received = USART1->DR;
        	        	handler->RxBuffer[handler->RxHead] = received;
        	        	handler->RxHead = nextHead;

        	        	if (ptr_fun[0]) {
        	        	    ptr_fun[0](received);
        	        	}
        } else {
            volatile uint8_t dummy = USART1->DR; // Drop data
        }
    }
}
