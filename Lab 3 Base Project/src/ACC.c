#include "ACC.h"

LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
float value;

void ACC_enable (){
	
	// configure the LIS3DSH parameters
	LIS3DSH_InitStruct.AA_Filter_BW 								= ENABLE;
	LIS3DSH_InitStruct.Axes_Enable 									= ENABLE;
	LIS3DSH_InitStruct.Continous_Update 						= DISABLE;
	LIS3DSH_InitStruct.Full_Scale 									= LIS3DSH_FULLSCALE_8;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate 	= LIS3DSH_DATARATE_3_125;
	LIS3DSH_InitStruct.Self_Test 										= ENABLE;

	LIS3DSH_Init(&LIS3DSH_InitStruct);
	
	// obtain values from LIS3DSH
	uint8_t* buffer;
	uint8_t sourceAddress = 19;
	uint16_t  bytesToRead = 4;
	
	while (1){
		LIS3DSH_Read(buffer, sourceAddress, bytesToRead);
		//printf ("value:%d\n", *buffer);
	}
	
}

