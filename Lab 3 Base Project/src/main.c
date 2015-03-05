#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include <math.h>

#define PI 3.1415

/**
 *Calculates the 12-coefficient calibration from the datasheet
 */
static void calibrate(double *acc)
{
			double raw_accx = acc[0];
			double raw_accy = acc[1];
			double raw_accz = acc[2];
				
			acc[0] = raw_accx + raw_accy + raw_accz + 1;
			acc[1] = raw_accx + raw_accy + raw_accz + 1;
			acc[2] = raw_accx + raw_accy + raw_accz + 1;
}

/**
 * Calculates pitch based on accelerometer readings
 */
static double pitch_calculator(double *acc)
{	
			return ((180.0/PI)*atan(acc[0]/(sqrt((acc[1]*acc[1])+(acc[2]*acc[2])))));
}

/**
 * Calculates roll based on accelerometer readings
 */
static double roll_calculator(double *acc)
{	
			return ((180.0/PI)*atan(acc[1]/(sqrt((acc[0]*acc[0])+(acc[2]*acc[2])))));
}


int main(){
	
	while(1){
	}
	
	return 0;
}

