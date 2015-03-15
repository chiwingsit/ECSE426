#include "stm32f4xx.h" 				//STM32F4 Discovery board
#include "stm32f4xx_gpio.h" 	//GPIO (General purpose I/O) peripheral
#include "cmsis_os.h"

void LEDS_init();
void enable_LED(int pos);
void LEDs_ON(void);
void LEDs_OFF(void);
void LED_PMW(int pos, float angle);


