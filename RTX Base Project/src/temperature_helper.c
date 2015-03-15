/*
Group 8
references: 
- tutorial slides
- http://myembeddedtutorial.blogspot.ca/2013/12/working-with-stm32f4-temperature-sensor.html
*/

#include "temperature_helper.h"
#include "KalmanFilter.h"

// INITIALIZE KALMAN STATE
// with initial values
kalman_state kstate = {
	0.001,         // q
	3.821383247,	 // r
	1048,          // x
	1,             // p
	0              // k
};

// CONFIGURE ADC
void ADC_init(){

		// initialize ADC
		ADC_InitTypeDef adc_init_s;			
		ADC_CommonInitTypeDef adc_common_init_s;

		// configure ADC sampling settings
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // clock gating
		adc_common_init_s.ADC_Mode 							= ADC_Mode_Independent;       	// independent mode
		adc_common_init_s.ADC_Prescaler 				= ADC_Prescaler_Div8;  					// clock frequency
		adc_common_init_s.ADC_DMAAccessMode 		= ADC_DMAAccessMode_Disabled;		// no DMA
		adc_common_init_s.ADC_TwoSamplingDelay 	= ADC_TwoSamplingDelay_5Cycles; // delay between 2 sampling phases
		ADC_CommonInit(&adc_common_init_s); // initialization

		// configure ADC settings
		adc_init_s.ADC_Resolution 					= ADC_Resolution_12b; // 12-bit resolution
		adc_init_s.ADC_ScanConvMode 				= DISABLE;
		adc_init_s.ADC_ContinuousConvMode 	= DISABLE;
		adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		adc_init_s.ADC_DataAlign 						= ADC_DataAlign_Right;
		adc_init_s.ADC_NbrOfConversion 			= 1;
		ADC_Init(ADC1, &adc_init_s); 				// initialization

		ADC_RegularChannelConfig(ADC1, ADC_Channel_16,
		1, ADC_SampleTime_480Cycles); 		// configure: channel (16), rank, sample time
		ADC_TempSensorVrefintCmd(ENABLE); // enable temperature sensor
		ADC_Cmd(ADC1, ENABLE); 						// enable ADC1
}

// READ TEMPERATURE
// returns a temperature reading (°C) from sensor
float getTemp (){

		float TemperatureValue;
	
		// obtain value from ADC
		ADC_SoftwareStartConv(ADC1); 														//start conversion
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //wait for conversion to process
		ADC_ClearFlag (ADC1, ADC_FLAG_EOC);											//end of conversion
		TemperatureValue = ADC_GetConversionValue(ADC1); 				//return converted value
	
		// filter ADC output value with Kalman filter
		Kalmanfilter_C(TemperatureValue, &kstate);
		//printf ("%f\t%f\n",TemperatureValue,kstate.x);
		TemperatureValue = kstate.x;
		
		// value converted from voltage to °C	(reference: STM32F407VG datasheet)
		TemperatureValue *= 3000; 		// voltage range 0 to 3000 mV
		TemperatureValue /= 4095; 		// divide by 12-bits of resolution -> reading in mV
		TemperatureValue /= 1000.0; 	// reading in Volts
		TemperatureValue -= 0.760; 		// subtract reference voltage for 25°C
		TemperatureValue /= .0025; 		// divide by 2.5 mV/°C
		TemperatureValue += 25.0; 		// add back 25°C
		
		return TemperatureValue;
}

