#ifndef _RCC_INTERFACE_H
#define _RCC_INTERFACE_H

void RCC_voidAHB1_PeripheralEnable(uint8_t copy_u8peripheral);
void RCC_voidAHB2_PeripheralEnable(uint8_t copy_u8peripheral);
void RCC_voidAHB3_PeripheralEnable(uint8_t copy_u8peripheral);
void RCC_voidAHB1_PeripheralDisable(uint8_t copy_u8peripheral);
void RCC_voidAHB2_PeripheralDisable(uint8_t copy_u8peripheral);
void RCC_voidAHB3_PeripheralDisable(uint8_t copy_u8peripheral);

void RCC_voidAPB2_PeripheralEnable(uint8_t copy_u8peripheral);
void RCC_voidAPB2_PeripheralDisable(uint8_t copy_u8peripheral);
void RCC_voidAPB1_PeripheralEnable(uint8_t copy_u8peripheral);
void RCC_voidAPB1_PeripheralDisable(uint8_t copy_u8peripheral);



#endif /* _RCC_INTERFACE_H */



