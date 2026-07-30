#include "RCC_Register.h"
#include "RCC_interface.h"

void RCC_voidAHB1_PeripheralEnable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_AHB1ENR |= (0x1<<copy_u8peripheral);
}
void RCC_voidAHB2_PeripheralEnable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_AHB2ENR |= (0x1<<copy_u8peripheral);
}
void RCC_voidAHB3_PeripheralEnable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_AHB3ENR |= (0x1<<copy_u8peripheral);
}
void RCC_voidAHB1_PeripheralDisable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_AHB1ENR &= ~(0x1<<copy_u8peripheral);
	
}
void RCC_voidAHB2_PeripheralDisable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_AHB2ENR &= ~(0x1<<copy_u8peripheral);
}
void RCC_voidAHB3_PeripheralDisable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_AHB3ENR &= ~(0x1<<copy_u8peripheral);
}
void RCC_voidAPB2_PeripheralEnable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_APB2ENR |= (0x1<<copy_u8peripheral);
}
void RCC_voidAPB2_PeripheralDisable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_APB2ENR &= ~(0x1<<copy_u8peripheral);
}
void RCC_voidAPB1_PeripheralEnable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_APB1ENR |= (0x1<<copy_u8peripheral);
}
void RCC_voidAPB1_PeripheralDisable(uint8_t copy_u8peripheral)
{
	if (copy_u8peripheral < 32) RCC->RCC_APB1ENR &= ~(0x1<<copy_u8peripheral);
}



