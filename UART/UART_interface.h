#ifndef _UART_INTERFACE_H
#define _UART_INTERFACE_H
#include "UART_Register.h"
#define TIME_OUT 10000U

#define USART_CR1_UE     (1 << 13)  // USART enable
#define USART_CR1_M      (1 << 12)  // Word length
#define USART_CR1_PCE    (1 << 10)  // Parity control enable
#define USART_CR1_PS     (1 << 9)   // Parity selection
#define USART_CR1_TE     (1 << 3)   // Transmitter enable
#define USART_CR1_RE     (1 << 2)   // Receiver enable

// Correct interrupt enable bits for USART_CR1
#define USART_TXE_INTERRUPT_ENABLE   (1 << 7)  // TXEIE – Transmit Data Register Empty interrupt enable
#define USART_TC_INTERRUPT_ENABLE    (1 << 6)  // TCIE  – Transmission Complete interrupt enable
#define USART_RXNE_INTERRUPT_ENABLE  (1 << 5)  // RXNEIE – Receive Data Register Not Empty interrupt enable


#define USART_SR_TXE     (1 << 7)   // Transmit data register empty
#define USART_SR_TC      (1 << 6)   // Transmission complete
#define USART_SR_RXNE    (1 << 5)   // Read data register not empty
#define USART_SR_ORE     (1 << 3)   // Overrun error
#define USART_SR_NE      (1 << 2)   // Noise error
#define USART_SR_FE      (1 << 1)   // Framing error
#define USART_SR_PE      (1 << 0)   // Parity error



#define USART_CR3_DMAR   (1 << 6)   // DMA enable receiver
#define USART_CR3_DMAT   (1 << 7)   // DMA enable transmitter

#define USART_MODE_RX      (1 << 2)
#define USART_MODE_TX      (1 << 3)
#define USART_MODE_RX_TX   ((1 << 2) | (1 << 3))

typedef enum {
	OVER_SAMPLING_DIV16=0,OVER_SAMPLING_DIV8
}Sampling_Type_t;
typedef enum {
	Data_bits_8=0,Data_bits_9
}Word_Length_t;
typedef enum {
	one_Stop_bit=0,Half_Stop_bit,Two_Stop_bit,One_and_Half_Stop_bit
}Stop_Bit_Number_t;
typedef enum {
	USART_HW_NONE=0, USART_HW_RTS, USART_HW_CTS, USART_HW_RTS_CTS
}USART_HWFC_t;

typedef enum {
	USART_PARITY_NONE=0,USART_PARITY_EVEN,USART_PARITY_ODD
}USART_PARITY_t;
typedef struct {
	USART_RegDef_t* USART_Instance;
    uint32_t BaudRate;
    Word_Length_t DataBits;
    Stop_Bit_Number_t StopBits;
    USART_PARITY_t Parity; //USART_PARITY_NONE ,USART_PARITY_EVEN
    uint8_t Mode; // USART_MODE_RX ,USART_MODE_TX , USART_MODE_RX_TX
    USART_HWFC_t HWFlowControl;
    Sampling_Type_t Sampling;
    void (*ptr_callback)(uint8_t);
} USART_Config_t;

void UART_Init(const USART_Config_t* config);
void USART_SendData(USART_RegDef_t* USARTx, uint8_t* data, uint16_t len);
uint32_t USART_ReceiveData(USART_RegDef_t* USARTx, uint8_t* buffer, uint16_t len);
uint32_t USART_ReceiveData_word(USART_RegDef_t* USARTx, uint8_t* buffer, uint16_t len);

void UART_Init_IT(const USART_Config_t* config);
void USART_RegisterHandler(USART_RegDef_t* USARTx) ; // before Initalization ( Enable also the Vector NVIC )
void USART_SendData_IT(USART_RegDef_t* USARTx, uint8_t* data, uint16_t len);
void USART_Read_IT(USART_RegDef_t* USARTx, uint8_t* data);


void USART_Init_DMA(const USART_Config_t* config);
void USART2_Receive_DMA(uint8_t *pData, uint32_t len);
void USART2_Transmit_DMA(uint8_t *pData, uint32_t len);// IN the DMA1_Stream6_IRQHandler() CALL THE Clear Flags in DMA DEIVER

#endif /* _UART_INTERFACE_H */
