// INCLUDES --------------------------------------------------------------------------------------
#include "stdio.h"
#include "GPIO_helper.h"
#include "temperature_helper.h"

unsigned int ticks = 0; //for SysTick timer

// MAIN ---------- -------------------------------------------------------------------------------
int main(void){
	
		GPIO_init();
		temp_init();
	
		float previousTemp = getTemp();
		float currentTemp;
		int position = 0;
		enable_LED(position);
		
		int sampling = 0;
		
		int PWM_counter = 0;
		int duty_cycle = 0;
		int mode = 0;
		int max_period = 15;
		
		//SysTick timer and obtain Temp reading ------------------------------------------------------
		
		while(1){		 
			ticks = 0;
			// Configure for 1 kHz period
			SysTick_Config(SystemCoreClock / 1000); //Number of ticks between two interrupts
			// or 1/Freq * SystemCoreClock
			while(1){
				// Wait for an interrupt
				while(!ticks);
				// Decrement ticks
				ticks = 0;
				
				sampling++;
				// read temperature from the sensor
				if(sampling == 20){
					currentTemp = getTemp ();
					sampling = 0;
					//printf ("%d\t%f\t%f\n", position, previousTemp, currentTemp);
				}
				
				if(currentTemp > 60){
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

//Interrupt handler for system tick
void SysTick_Handler(){
	ticks = 1;
}

