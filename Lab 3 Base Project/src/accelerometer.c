#include "accelerometer.h"

LIS3DSH_InitTypeDef LIS3DSH_InitStruct;

void accelerometer_enable (){
	
	// configure theACC parameters
	LIS3DSH_InitStruct.AA_Filter_BW 								= ENABLE;
	LIS3DSH_InitStruct.Axes_Enable 									= ENABLE;
	LIS3DSH_InitStruct.Continous_Update 						= ENABLE;
	LIS3DSH_InitStruct.Full_Scale 									= LIS3DSH_FULLSCALE_2;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate 	= LIS3DSH_DATARATE_3_125;
	LIS3DSH_InitStruct.Self_Test 										= ENABLE;

	LIS3DSH_Init(&LIS3DSH_InitStruct);
}