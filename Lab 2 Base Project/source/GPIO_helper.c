#include "GPIO_helper.h"

GPIO_InitTypeDef  GPIO_InitStruct;

void GPIO_init(){
		//enable I/O ---------------------------------------------------------------------------------

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //enables the peripheral clock to the GPIOD module

		//GPIO_InitStruct.GPIO_Pin configures the pins that will be used.
		//In this case we will use the LED's off of the discovery board which are on
		//PortD pins 12, 13, 14 and 15
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12;

		//PIO_InitStruct.GPIO_Mode configures the pin mode the options are as follows
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

		//GPIO_InitStruct.GPIO_Speed configures the clock speed, options are as follows
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

		//GPIO_InitStruct.GPIO_OType configures the pin type, options are as follows
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

		//Configures pullup / pulldown resistors on pin, options are as follows
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

		//This finally passes all the values to the GPIO_Init function
		//which takes care of setting the corresponding bits.
		GPIO_Init(GPIOD, &GPIO_InitStruct);

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		/**********************************************************************************
		*
		* This block of code defines the properties of the GPIOA port.
		* We are defining Pin 0 as a digital input with a pulldown resistor
		* to detect a high level.  Pin 0 is connected to the 3.3V source
		*
		**********************************************************************************/

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

void enable_LED(int pos) 
{
		BitAction LED0 = Bit_RESET;
		BitAction LED1 = Bit_RESET;
		BitAction LED2 = Bit_RESET;
		BitAction LED3 = Bit_RESET;
	
		switch(pos){
			case 0:
				LED0 = Bit_SET;
				break;
			case 1:
				LED1 = Bit_SET;
				break;
			case 2:
				LED2 = Bit_SET;
				break;
			case 3:
				LED3 = Bit_SET;
				break;
		}

		GPIO_WriteBit(GPIOD, GPIO_Pin_12, LED0);
		GPIO_WriteBit(GPIOD, GPIO_Pin_13, LED1);
		GPIO_WriteBit(GPIOD, GPIO_Pin_14, LED2);
		GPIO_WriteBit(GPIOD, GPIO_Pin_15, LED3);
}


void LEDs_ON(){
		GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
}

void LEDs_OFF(){
		GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET);
}

