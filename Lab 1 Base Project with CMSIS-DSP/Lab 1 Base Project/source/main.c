#include <stdio.h>
#include "arm_math.h"
#include "test.h"

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

//  arm_correlate_f32 (float32_t *pSrcA, uint32_t srcALen, float32_t *pSrcB, uint32_t srcBLen, float32_t *pDst);
//	arm_conv_f32 (	float32_t * 	pSrcA,uint32_t 	srcALen,float32_t * 	pSrcB,uint32_t 	srcBLen,float32_t * 	pDst )		
		

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
	kalman_state *kstate = (kalman_state*) malloc(sizeof(kalman_state));
	kstate->q = 0.1;
	kstate->r = 0.1;
	kstate->p = 0.1;
	kstate->x = 0.0;
	
	float output_asm[10];
	float output_C[10];
	
	float *pIn;
	float *pOut;
	
	pIn = testArray;
	pOut = output_asm;
	
	Kalmanfilter_asm(pIn,pOut,kstate,10);
	
	pOut = output_C;
	kstate->q = 0.1;
	kstate->r = 0.1;
	kstate->p = 0.1;
	kstate->x = 0.0;
	
	Kalmanfilter_C(pIn,pOut,kstate,10);
	float *pointer_diff_C;
	float *pointer_diff_CMSIS;
	
	pointer_diff_C = subs_diff(pIn, pOut, 10);
	arm_sub_f32 ( pIn, pOut, pointer_diff_CMSIS, 10);
	
	float mean_C;
	float32_t mean_CMSIS;
	mean_C = average_diff(pointer_diff_C, 10);
	arm_mean_f32(pOut, 10, &mean_CMSIS);
	
	float std_C;
	float32_t std_CMSIS;
	std_C = standard_deviation(pointer_diff_C, 10);
	arm_std_f32(pointer_diff_CMSIS, 10, &std_CMSIS);
	
	float32_t *pDst;
	correlation(pIn,	pOut, 10);
	arm_correlate_f32 (pIn, 10, pOut, 10, pDst);
	
	convolution(pIn, pOut,10);
	arm_conv_f32 (pIn, 10, pOut, 10, pDst);	
	
	while(1);
	return 0;
}




