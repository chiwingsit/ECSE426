/*
Group 8
*/

#include "stdio.h"
#include "GPIO_helper.h"
#include "temperature_helper.h"

unsigned int ticks = 0; //for SysTick timer

// MAIN
int main(void){
	
		GPIO_init();
		ADC_init();
	
		float previousTemp = getTemp();
		float currentTemp;
		int position = 0;
		enable_LED(position);
		
		int sampling = 0;
		
		int PWM_counter = 0;
		int duty_cycle = 0;
		int mode = 0;
		int max_period = 18;
		
		//SysTick timer		
		while(1){		 
			ticks = 0;
			// 	configure for 1 kHz period
			//	number of ticks between two interrupts
			// 	or 1/Freq * SystemCoreClock
			SysTick_Config(SystemCoreClock / 1000); 
		
			while(1){
				while(!ticks);						// Wait for an interrupt
				ticks = 0;								// Decrement ticks
				sampling++;
				if(sampling == 20){ 			// read temperature from the sensor
					currentTemp = getTemp ();
					sampling = 0;
					//printf ("%d\t%f\t%f\n", position, previousTemp, currentTemp);
					printf ("%f\n", currentTemp);
				}
				
				// high temperature LED warning alarm
				if(currentTemp > 60){
					previousTemp = 62;
					PWM_counter++;
					if(duty_cycle == max_period){
						mode = 1;
					}
					else if(duty_cycle == 0){
						mode = 0;
					}
					if(PWM_counter == max_period){
						PWM_counter = 0;
						switch(mode){
							case 0:
								duty_cycle += 1;
							break;
							case 1:
								duty_cycle -= 1;
							break;
						}
					}
					
					if(PWM_counter < duty_cycle){
						LEDs_ON();
					}
					else if(PWM_counter > duty_cycle){
						LEDs_OFF();
					}					
				}
				// rotating LED temperature range display
				else if(currentTemp < previousTemp - 2){
					previousTemp = currentTemp;
					position = (position + 3) % 4;
					enable_LED(position);
				}
				else if(currentTemp > previousTemp + 2){
					previousTemp = currentTemp;
					position = (position + 1) % 4;
					enable_LED(position);
				}
			}
		}
}

// INTERRUPT HANDLER FOR SYSTICK TIMER
void SysTick_Handler(){
	ticks = 1;
}

