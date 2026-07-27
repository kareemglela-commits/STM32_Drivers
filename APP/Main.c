/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Kareem Glela
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "RCC_interface.h"
#include "GPIO_interface.h"
int main(void)
{

	 RCC_voidAHB1_PeripheralEnable(0);
	 RCC_voidAHB1_PeripheralEnable(1);
	 RCC_voidAHB1_PeripheralEnable(2);
	 RCC_voidAHB1_PeripheralEnable(3);
	 GPIO_PIN_CONFIG LED1=
	 {
			 .GPIOx= GPIOB,
			 .Pin_dir=GPIO_PIN_OUTPUT,
			 .GPIO_Pin_num=GPIO_PIN_0,
			 .out_speed= GPIO_PIN_OUT_MS,
			 .out_type=GPIO_PIN_OUT_PP,
			 .pin_state=GPIO_PIN_OUT_HIGH,
			 .pull_type=GPIO_PIN_NOPULL
	 };
	 GPIO_PIN_CONFIG LED2=
	 {
			 .GPIOx= GPIOA,
			 .Pin_dir=GPIO_PIN_OUTPUT,
			 .GPIO_Pin_num=GPIO_PIN_4,
			 .out_speed= GPIO_PIN_OUT_HS,
			 .out_type=GPIO_PIN_OUT_PP,
			 .pin_state=GPIO_PIN_OUT_HIGH,
			 .pull_type=GPIO_PIN_NOPULL
	 };
	 GPIO_PIN_CONFIG BTN1=
	 {
			 .GPIOx= GPIOC,
			 .Pin_dir=GPIO_PIN_INPUT,
			 .GPIO_Pin_num=GPIO_PIN_1,
			 .pull_type=GPIO_PIN_PULLUP
	 };
	 GPIO_PIN_Init(&LED1);
	 GPIO_PIN_Init(&LED2);
	 GPIO_PIN_Init(&BTN1);
	 uint8_t Button_Status ;
	for(;;)
	{
		Button_Status = GPIO_PIN_Read(GPIOC,GPIO_PIN_1);
		if(Button_Status == 0x1)
		{
			GPIO_PIN_Write(GPIOB, GPIO_PIN_0, GPIO_PIN_OUT_HIGH);
			GPIO_PIN_Write(GPIOA, GPIO_PIN_4, GPIO_PIN_OUT_LOW);
		}else
		{
			GPIO_PIN_Write(GPIOB, GPIO_PIN_0, GPIO_PIN_OUT_LOW);
			GPIO_PIN_Write(GPIOA, GPIO_PIN_4, GPIO_PIN_OUT_HIGH);
		}



	}
}
