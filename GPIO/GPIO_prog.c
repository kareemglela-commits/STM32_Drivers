#include <stdint.h>
#include "GPIO_Register.h"
#include "GPIO_interface.h"


void GPIO_PIN_Init(GPIO_PIN_CONFIG* pin_config)
{
	GPIO_RegDef_t* Gpio = pin_config->GPIOx;
	if(pin_config->Pin_dir == GPIO_PIN_INPUT)
		{
			Gpio->GPIO_MODER &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
			
			Gpio->GPIO_PUPDR &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
			Gpio->GPIO_PUPDR |=((pin_config->pull_type)<<(2*(pin_config->GPIO_Pin_num)));		
		}
	else if(pin_config->Pin_dir == GPIO_PIN_OUTPUT)
	{
		Gpio->GPIO_MODER &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
		Gpio->GPIO_MODER |=((pin_config->Pin_dir)<<(2*(pin_config->GPIO_Pin_num)));
		
		Gpio->GPIO_OTYPER &=~(0x1<<(pin_config->GPIO_Pin_num));
		Gpio->GPIO_OTYPER |=((pin_config->out_type)<<(pin_config->GPIO_Pin_num));
		
		Gpio->GPIO_OSPEEDER &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
		Gpio->GPIO_OSPEEDER |=((pin_config->out_speed)<<(2*(pin_config->GPIO_Pin_num)));

		Gpio->GPIO_ODR &=~(0x1<<((pin_config->GPIO_Pin_num)));
		Gpio->GPIO_ODR |=((pin_config->pin_state)<<((pin_config->GPIO_Pin_num)));
	}
	else if(pin_config->Pin_dir == GPIO_PIN_AF)
	{
		Gpio->GPIO_MODER &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
		Gpio->GPIO_MODER |=((pin_config->Pin_dir)<<(2*(pin_config->GPIO_Pin_num)));
		
		Gpio->GPIO_PUPDR &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
		Gpio->GPIO_PUPDR |=((pin_config->pull_type)<<(2*(pin_config->GPIO_Pin_num)));
		
		Gpio->GPIO_OTYPER &=~(0x1<<(pin_config->GPIO_Pin_num));
		Gpio->GPIO_OTYPER |=((pin_config->out_type)<<(pin_config->GPIO_Pin_num));
		
		Gpio->GPIO_OSPEEDER &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
		Gpio->GPIO_OSPEEDER |=((pin_config->out_type)<<(2*(pin_config->GPIO_Pin_num)));
		if(pin_config->GPIO_Pin_num <= GPIO_PIN_7)
		{
			Gpio->GPIO_AFR[0] &=~(0xF<<(4*(pin_config->GPIO_Pin_num)));
			Gpio->GPIO_AFR[0] |=((pin_config->ALF_num)<<(4*(pin_config->GPIO_Pin_num)));
		}
		else 
		{
			Gpio->GPIO_AFR[1] &=~(0xF<<(4*(pin_config->GPIO_Pin_num)-32));
			Gpio->GPIO_AFR[1] |=((pin_config->ALF_num)<<(4*(pin_config->GPIO_Pin_num)-32));
		}
		
	}
	else if(pin_config->Pin_dir == GPIO_PIN_ANALOG)
	{
		Gpio->GPIO_MODER &=~(0x3<<(2*(pin_config->GPIO_Pin_num)));
		Gpio->GPIO_MODER |=((pin_config->Pin_dir)<<(2*(pin_config->GPIO_Pin_num)));
	}
}


void GPIO_PIN_Write(GPIO_RegDef_t* Gpio, GPIO_PIN_NUM pin_num, GPIO_PIN_OUT_STATE u8_state)
{
    if(pin_num <= GPIO_PIN_15)
    {
        if(u8_state == GPIO_PIN_OUT_HIGH)
            Gpio->GPIO_ODR |= (0x1 << pin_num);
        else if(u8_state == GPIO_PIN_OUT_LOW)
            Gpio->GPIO_ODR &= ~(0x1 << pin_num);
    }
}

void GPIO_PIN_Toggle(GPIO_RegDef_t* Gpio, GPIO_PIN_NUM pin_num)
{
    if(pin_num <= GPIO_PIN_15)
    {
        uint8_t Current_State = (Gpio->GPIO_ODR >> pin_num) & 0x1;
        if(Current_State == GPIO_PIN_OUT_HIGH)
            Gpio->GPIO_ODR &= ~(0x1 << pin_num);
        else
            Gpio->GPIO_ODR |= (0x1 << pin_num);
    }
}
uint8_t GPIO_PIN_Read(GPIO_RegDef_t* Gpio,GPIO_PIN_NUM pin_num)
{
	return (Gpio->GPIO_IDR >> pin_num) & 0x1;
}
