#include <stdio.h>
#include "arm_math.h"
#include "test.h"
#include "array.h"

//typedef signed long int32_t;

typedef struct
{
	float q;
	float r;
	float x;
	float p;
	float k;
}kalman_state;

extern int Kalmanfilter_asm (float* InputArray, float* OutputArray, kalman_state* kstate, int length);

float* subs_diff(float* inputArray, float* outputArray, int n){
	int i;
	float subArray[10];
	for (i = 0; i < n; i++)
	{	
		/*substraction of original and output */
		subArray[i] = inputArray[i] - outputArray[i];	
	}
	return subArray;
}

float average_diff(float* x, int n){
	int i;
	float sum=0, average;
	for (i = 0; i < n; i++)
	{
			sum = sum + x[i];
	}
	average = sum / (float)n;
	return average;
		
}

float standard_deviation(float* data, int n){
	float mean=0.0, sum_deviation=0.0;
	int i;
	for(i=0; i<n;++i)
	{
		mean+=data[i];
	}
	mean=mean/n;
	for(i=0; i<n;++i)
	sum_deviation+=(data[i]-mean)*(data[i]-mean);
	return sqrt(sum_deviation/n); 
				
}
float* correlation(float* inputArray, float* outputArray, int n){
	float r[n];
	int i;
	for (i = 0; i < n; i++)
	{
		r[i] = inputArray[i] * outputArray[n-i];
	}
	return r;
}

float* convolution(float* inputArray, float* outputArray, int n){
	float r[n];
	int i;
	for (i = 0; i < n; i++)
	{
		r[i] = inputArray[i] * outputArray[i];
	}
	return r;
}

		

int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int length)
{
	int i;
	for(i = 0; i < length; i++){
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		kstate->x = kstate->x + kstate->k * (InputArray[i]-kstate->x);
		kstate->p = (1-kstate->k) * kstate->p;
		OutputArray[i] = kstate->x;
	}
	return 0;
}

int main()
{
	int length = sizeof(testArray)/sizeof(testArray[0]);
	
	kalman_state *kstate = (kalman_state*) malloc(sizeof(kalman_state));
	kstate->q = 0.1;
	kstate->r = 0.1;
	kstate->p = 0.1;
	kstate->x = 0.0;
	
	float output_asm[length];
	float output_C[length];
	
	float *pIn;
	float *pOut;
	
	pIn = testArray;
	pOut = output_asm;
	
	Kalmanfilter_asm(pIn,pOut,kstate,length);

	
	pOut = output_C;
	kstate->q = 0.1;
	kstate->r = 0.1;
	kstate->p = 0.1;
	kstate->x = 0.0;
	
	Kalmanfilter_C(pIn,pOut,kstate,length);
	
		
	for(int i = 0; i < length; i++)
	{
		printf("%f\t%f\n", output_asm[i], output_C[i]);
	}

	
	float *pdiff_C;
	float *pdiff_CMSIS;
	pdiff_C = subs_diff(pIn, pOut, length);
	arm_sub_f32 ( pIn, pOut, pdiff_CMSIS, length);
	
	float mean_C;
	float32_t mean_CMSIS;
	mean_C = average_diff(pdiff_C, length);
	arm_mean_f32(pdiff_CMSIS, 10, &mean_CMSIS);
	
	float std_C;
	float32_t std_CMSIS;
	std_C = standard_deviation(pdiff_C, length);
	arm_std_f32(pdiff_CMSIS, length, &std_CMSIS);
	
	float32_t *pDst;
	correlation(pIn,	pOut, length);
	arm_correlate_f32 (pIn, length, pOut, length, pDst);
	
	float32_t *pCov;
	convolution(pIn, pOut,length);
	arm_conv_f32 (pIn, length, pOut, length, pCov);
	
	while(1);
	return 0;
}




