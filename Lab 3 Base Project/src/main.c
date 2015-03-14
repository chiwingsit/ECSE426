#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "segment_display.h"
#include "MEMS.h"
#include "timer.h"

int SAMPLE_READY = 0;

int main(){
	seg_disp_init();
	MEMS_config();
	MEMS_interrupt_config();
	
	int input = display_keypad_input();
	
	input = input % 180;
	
	float angle;
	
	while(1){
		if(SAMPLE_READY){
			SAMPLE_READY = 0;
			angle = get_pitch();
			if(angle < 0){
				angle = (-1) * angle;
			}
			//pitch = (int) (pitch + 360) % 180;
			printf("%f\n", angle);
		}

		if(angle > input + 5){
			display_low();
		}
		else if(angle < input - 5 ){
			display_high();
		}
		else{
			display_angle(angle);
		}
	}
	
	return 0;
}


void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){
		// clear the flag in the sensor's end
		SAMPLE_READY = 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


