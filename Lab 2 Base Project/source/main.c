// INCLUDES --------------------------------------------------------------------------------------
#include "stm32f4xx.h" 				//STM32F4 Discovery board
#include "stm32f4xx_gpio.h" 	//GPIO (General purpose I/O) peripheral
#include "stm32f4xx_adc.h" 		// A/D converter
#include "stdio.h"
#include "GPIO_helper.h"

unsigned int ticks = 0; //for SysTick timer
float TemperatureValue = 0;	

//initialize A/D converter
ADC_InitTypeDef ADC_InitStruct;			
ADC_CommonInitTypeDef ADC_CommonInitStruct;

typedef struct
{
	float q;
	float r;
	float x;
	float p;
	float k;
}kalman_state;

kalman_state kstate_temp;

// Kalman Filter
void Kalmanfilter_C(float measurement, kalman_state* kstate)
{
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		kstate->x = kstate->x + kstate->k * (measurement-kstate->x);
		kstate->p = (1-kstate->k) * kstate->p;
}

// READ TEMPERATURE -------------------------------------------------------------------------------
//returns a temperature reading (Celsius) from sensor
float getTemp (){
	
		//enable A/D converter to sample the on board sensor ------------------------------------------
	
		ADC_DeInit(); //Deinitialize the ADC

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent; //Configures the ADC to operate in independent or multi mode.
		ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8; //Select the frequency of the clock to the ADC. The clock is common for all the ADCs.
		ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //Configures the Direct memory access mode for multi ADC mode.
		ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //Configures the Delay between 2 sampling phases.
		ADC_CommonInit(&ADC_CommonInitStruct); //Two Sampling Delay = # * Time(ADCCLK) where # is between 5 and 20

		//Configure The ADC Initialization Settings
		ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b; 		//12-bit resolution
		ADC_InitStruct.ADC_ScanConvMode = DISABLE;
		ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStruct.ADC_NbrOfConversion = 1;
		ADC_Init(ADC1, &ADC_InitStruct);

		// ADC1 Configuration, ADC_Channel_TempSensor is actual channel 16
		ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1,
		ADC_SampleTime_144Cycles);

		// Enable internal temperature sensor
		ADC_TempSensorVrefintCmd(ENABLE);

		// Enable ADC conversion
		ADC_Cmd(ADC1, ENABLE);

		//use A/D converter, then convert value to celsius ------------------------------------------
		
		// ADC Conversion to read temperature sensor
		// Temperature (in °C) = ((Vsense – V25) / Avg_Slope) + 25
		// Vense = Voltage Reading From Temperature Sensor
		// V25 = Voltage at 25°C, for STM32F407 = 0.76V
		// Avg_Slope = 2.5mV/°C
		// This data can be found in the STM32F407VF Data Sheet

		ADC_SoftwareStartConv(ADC1); //Start the conversion
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		; //Processing the conversion
		TemperatureValue = ADC_GetConversionValue(ADC1); //Return the converted data
		TemperatureValue *= 3000; //voltage range 0 to 3 V
		TemperatureValue /= 4095; //divide by 12-bits of resolution -> Reading in mV
		TemperatureValue /= 1000.0; //Reading in Volts
		TemperatureValue -= 0.760; // Subtract the reference voltage at 25°C
		TemperatureValue /= .0025; // Divide by slope 2.5mV
		TemperatureValue += 25.0; // Add the 25°C
		
		return TemperatureValue;
}



// MAIN ---------- -------------------------------------------------------------------------------
int main(void){
	
		GPIO_init();
	
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
				}
				
				//printf ("%d\t%f\t%f\n", position, previousTemp, currentTemp);
				if(currentTemp > 60){
					PWM_counter++;
					if(duty_cycle == max_period){
						mode = 1;
					}
					else if(duty_cycle == 0){
						mode = 0;
					}
					if(PWM_counter == max_period){
						switch(mode){
							PWM_counter = 0;
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