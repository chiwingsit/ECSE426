#include "temperature_helper.h"
#include "KalmanFilter.h"

//initialize A/D converter
ADC_InitTypeDef ADC_InitStruct;			
ADC_CommonInitTypeDef ADC_CommonInitStruct;


kalman_state kstate = {
	0.001,
	3.821383247,
	1048,
	1,
	0
};


void temp_init(){
			
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
}

// READ TEMPERATURE -------------------------------------------------------------------------------
//returns a temperature reading (Celsius) from sensor
float getTemp (){

		//use A/D converter, then convert value to celsius ------------------------------------------
		
		// ADC Conversion to read temperature sensor
		// Temperature (in °C) = ((Vsense – V25) / Avg_Slope) + 25
		// Vense = Voltage Reading From Temperature Sensor
		// V25 = Voltage at 25°C, for STM32F407 = 0.76V
		// Avg_Slope = 2.5mV/°C
		// This data can be found in the STM32F407VF Data Sheet
		float TemperatureValue;
		ADC_SoftwareStartConv(ADC1); //Start the conversion
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		; //Processing the conversion
		TemperatureValue = ADC_GetConversionValue(ADC1); //Return the converted data
		Kalmanfilter_C(TemperatureValue, &kstate);
		printf ("%f\t%f\n",TemperatureValue,kstate.x);
		TemperatureValue *= 3000; //voltage range 0 to 3 V
		TemperatureValue /= 4095; //divide by 12-bits of resolution -> Reading in mV
		TemperatureValue /= 1000.0; //Reading in Volts
		TemperatureValue -= 0.760; // Subtract the reference voltage at 25°C
		TemperatureValue /= .0025; // Divide by slope 2.5mV
		TemperatureValue += 25.0; // Add the 25°C
		
		return TemperatureValue;
}

