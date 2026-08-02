#include "stdint.h"
#include "stdio.h"
#include "NVIC_Register.h"
#include "NVIC_Interface.h"



void NVIC_voidPeripheralEnable(uint8_t 	IRQ_num)
{
	/* we will use the atomic Acess Enable a prephiral Interrupt */
	uint8_t temp=IRQ_num;
	temp /=32;					  /*for more efficency uint8_t temp = IRQ_num>>5;	*/
	uint8_t IRQ_pin=IRQ_num %32; /*for more efficency uint8_t IRQ_pin=IRQ_num &0x1F;*/
	NVIC->NVIC_ISER[temp] = 1<<IRQ_pin;

}
void NVIC_voidPeripheralDisable(uint8_t IRQ_num)
{

	/* we will use the atomic Acess For Disable a prephiral Interrupt*/
	uint8_t temp=IRQ_num;
	temp /=32;
	uint8_t IRQ_pin=IRQ_num %32;
	NVIC->NVIC_ICER[temp] = 1<<IRQ_pin;
}