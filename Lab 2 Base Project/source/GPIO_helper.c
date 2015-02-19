/*
Group 8
references: 
- tutorial slides
- http://myembeddedtutorial.blogspot.ca/2013/12/
	working-with-stm32f4-temperature-sensor.html
*/

#include "GPIO_helper.h"

GPIO_InitTypeDef  GPIO_InitStruct;

// ENABLE I/O FOR LEDS
void GPIO_init(){
	
		// enables the clock to the GPIO bank D module (LEDs)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

		// configure pins to be used (LEDs)
		GPIO_InitStruct.GPIO_Pin = 
		GPIO_Pin_15 |  // blue LED
		GPIO_Pin_14 |  //	red LED
		GPIO_Pin_13 |  //	orange LED
		GPIO_Pin_12;   //	green LED
	
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;			// configure pin mode for output
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	// configure the clock speed to avoid limiting slew rate
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// configure the pin type to push-pull
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		// configure pullup/pulldown resistors on pin (not input, no pull)
		
		GPIO_Init(GPIOD, &GPIO_InitStruct);							// configure pin (pass values and set bits)
}

void enable_LED(int pos) 
{
		// reset all LEDs
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

		// write corresponding bits to LEDs
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, LED0);
		GPIO_WriteBit(GPIOD, GPIO_Pin_13, LED1);
		GPIO_WriteBit(GPIOD, GPIO_Pin_14, LED2);
		GPIO_WriteBit(GPIOD, GPIO_Pin_15, LED3);
}

// TURN ON ALL 4 LEDS
void LEDs_ON(){
		GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_SET);
}

// TURN OFF ALL 4 LEDS
void LEDs_OFF(){
		GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, Bit_RESET);
}

