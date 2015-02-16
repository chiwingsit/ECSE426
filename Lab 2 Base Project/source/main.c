// INCLUDES --------------------------------------------------------------------------------------
#include "stm32f4xx.h" 				//STM32F4 Discovery board
#include "stm32f4xx_gpio.h" 	//GPIO (General purpose I/O) peripheral
#include "stm32f4xx_adc.h" 		// A/D converter
#include "stdio.h"

unsigned int ticks = 0; //for SysTick timer
float TemperatureValue = 0;	

//initialize I/O
GPIO_InitTypeDef  GPIO_InitStruct; 

//initialize A/D converter
ADC_InitTypeDef ADC_InitStruct;			
ADC_CommonInitTypeDef ADC_CommonInitStruct;

// READ TEMPERATURE -------------------------------------------------------------------------------
//returns a temperature reading (Celsius) from sensor
void readTemp (){
	
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

			printf ("temp: %f \n", TemperatureValue);
}

// MAIN ---------- -------------------------------------------------------------------------------
int main(void){
	
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
		
		//SysTick timer and obtain Temp reading ------------------------------------------------------
		
			while(1){		 
					ticks = 0;
					// Configure for 20ms period (for 50 Hz)
					SysTick_Config(20 * SystemCoreClock / 1000); //Number of ticks between two interrupts
					// or 1/Freq * SystemCoreClock
					while(1){
						// Wait for an interrupt
						while(!ticks);
						// Decrement ticks
						ticks = 0;
						// read temperature from the sensor
						readTemp ();
					}
			}
}

//Interrupt handler for system tick
void SysTick_Handler(){
	ticks = 1;
}