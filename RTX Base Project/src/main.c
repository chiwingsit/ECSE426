/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "segment_display.h"
#include "MEMS.h"
#include "TIM.h"
#include "temperature_helper.h"
#include "LEDS_helper.h"
#include <stdio.h>

float temp;
float angle;
float display;
int mode;
int led;
uint32_t exec;
osTimerId timerId;

osMutexId temp_mutex;
osMutexDef(temp_mutex);
osMutexId angle_mutex;
osMutexDef(angle_mutex);

void Blinky_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}

void Blinky(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		disable_display();
		osTimerStop(timerId);
		osDelay(250);
		osTimerStart(timerId, 1);
		osDelay(250);
	}
}

osThreadDef(Blinky, osPriorityNormal, 1, 0);


void temp_thread(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		
		osMutexWait(temp_mutex, osWaitForever);
		temp = getTemp();
		//printf("temp: %f\n", temp);
		osMutexRelease(temp_mutex);
	}
}

osThreadId temp_thread_id;
osThreadDef(temp_thread, osPriorityNormal, 1, 0);



void angle_thread(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		
		osMutexWait(angle_mutex, osWaitForever);
		angle = get_pitch();
		if(angle < 0){
				angle = (-1) * angle;
		}
		//printf("angle: %f\n", angle);
		osMutexRelease(angle_mutex);
	}
}
osThreadId angle_thread_id;
osThreadDef(angle_thread, osPriorityNormal, 1, 0);

void keypad_thread()
{	
	while(1){
		char c = get_key();
		switch(c){
			case '#':
				mode = (mode + 1) %2;
				break;
			case '1':
				led = 0;
				break;
			case '2':
				led = 1;
				break;
			case '3':
				led = 2;
				break;
			case '4':
				led = 3;
				break;
		}
	}
}

osThreadId keypad_thread_id;
osThreadDef(keypad_thread, osPriorityNormal, 1, 0);


void Timer_Callback  (void const *arg)
{
	display_angle(display);
	//printf("display: %f\n", display);
}	

osTimerDef (Timer, Timer_Callback);

/*
 * main: initialize and start the system
 */
int main (void) {
	MEMS_config ();
	MEMS_interrupt_config();
	seg_disp_init();
	ADC_init();
	//timer_init();
	timer_temp_init();
	LEDS_init();
	
  osKernelInitialize (); // initialize CMSIS-RTOS
	
	temp_mutex = osMutexCreate(osMutex(temp_mutex));
	angle_mutex = osMutexCreate(osMutex(angle_mutex));
	// ID for thread
	osThreadId	Blinky_thread;
	
  // initialize peripherals here
	Blinky_GPIO_Init();
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	Blinky_thread = osThreadCreate(osThread(Blinky), NULL);
	temp_thread_id = osThreadCreate(osThread(temp_thread), NULL);
	angle_thread_id = osThreadCreate(osThread(angle_thread), NULL);
	keypad_thread_id = osThreadCreate(osThread(keypad_thread), NULL);
		
	exec = 1;
	timerId = osTimerCreate (osTimer(Timer), osTimerPeriodic, &exec);
	osStatus status = osTimerStart (timerId, 1);
	
	osKernelStart (); // start thread execution 
	
	while(1){
		if(temp > 60){
			osSignalSet(Blinky_thread, 0x01);
		}
		if(mode == 0){
			//osMutexWait(temp_mutex, osWaitForever);
			LEDs_OFF();
			display = temp;
			//osMutexRelease(temp_mutex);
		}
		else if(mode == 1){
			LED_PMW(led, angle);
			//osMutexWait(angle_mutex, osWaitForever);
			display = angle;
			//osMutexRelease(angle_mutex);
		}
	}

}

//	Interrupt generated at a frequency of 223(66/0.295) Hertz
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		osSignalSet(temp_thread_id, 0x01);
	}
}

void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){
		// clear the flag in the sensor's end
		osSignalSet(angle_thread_id, 0x01);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

