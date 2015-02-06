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

typedef int bool;
#define true 1
#define false 0

extern int Kalmanfilter_asm (float* InputArray, float* OutputArray, kalman_state* kstate, int length);

// Difference between two arrays
void subs_diff(float* inputArray, float* outputArray, float* diffArray, int n){
	int i;
	for (i = 0; i < n; i++)
	{	
		/*substraction of original and output */
		diffArray[i] = inputArray[i] - outputArray[i];	
	}
}

// Average
void average(float* x, int n, float *mean){
	int i;
	float sum=0;
	for (i = 0; i < n; i++)
	{
			sum = sum + x[i];
	}
	*mean = sum/n;
		
}

// Standard Deviation
void standard_deviation(float* data, int n, float *std){
	float mean, sum_deviation=0.0;
	
	average( data, n, &mean);
	
	int i;
	for(i=0; i<n;++i)
	{
		sum_deviation =sum_deviation + (data[i]-mean)*(data[i]-mean);
	}
	 *std = sqrt(sum_deviation/n); 
}

// Correlation
float* correlation(float* pSrcA, int srcALen, float* pSrcB, int srcBLen, float* pDst){
int i,k, kmin, kmax;
	for (i = 0; i < srcALen + srcBLen - 1; i++)
	{
		pDst[i] = 0;
		
		// Get the min index of the first array
		if(i < srcBLen - 1)
			kmin = 0;
		else
			kmin = i - (srcBLen - 1);
		
		// Get the max index of the first array
		if(i < srcALen - 1)
			kmax = i;
		else
			kmax = srcALen - 1;

    for (k = kmin; k <= kmax; k++)
    {
      pDst[i] += pSrcA[k] * pSrcB[srcBLen - (i - k) - 1];
    }
	}
}


// Convolution
void convolution(float* pSrcA, int srcALen, float* pSrcB, int srcBLen, float* pDst){
	int i,k, kmin, kmax;
	for (i = 0; i < srcALen + srcBLen - 1; i++)
	{
		pDst[i] = 0;
		
		// Get the min index of the first array
		if(i < srcBLen - 1)
			kmin = 0;
		else
			kmin = i - (srcBLen - 1);
		
		// Get the max index of the first array
		if(i < srcALen - 1)
			kmax = i;
		else
			kmax = srcALen - 1;

    for (k = kmin; k <= kmax; k++)
    {
      pDst[i] += pSrcA[k] * pSrcB[i - k];
    }
	}
}

// Test if two float two arrays are equal
bool areEqual(float *arrayA, float *arrayB, int length, float tol, char *err_message)
{
	
	for(int i = 0; i < length; i++)
	{
		if(arrayA[i] - arrayB[i] > tol || arrayA[i] - arrayB[i] < -tol)
		{
			printf("%s\n", err_message);
			return false;
		}
	}
	return true;
}
		

// Kalman Filter
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
	int length = sizeof(testVector)/sizeof(testVector[0]);
	// Initialize kstruct
	kalman_state *kstate = (kalman_state*) malloc(sizeof(kalman_state));
	kstate->q = 0.005;
	kstate->r = 5.0;
	kstate->p = 0.0;
	kstate->x = 0.0;
	
	float output_asm[length];
	float output_C[length];
	
	float *pIn;
	float *pOut;
	
	pIn = testVector;
	pOut = output_asm;
	
	// Assembly Kalman Filter
	Kalmanfilter_asm(pIn,pOut,kstate,length);
	
	// Initialize kstruct
	pOut = output_C;
	kstate->q = 0.005;
	kstate->r = 5.0;
	kstate->p = 0.0;
	kstate->x = 0.0;
	
	// C Kalman Filter
	Kalmanfilter_C(pIn,pOut,kstate,length);
	
	float tol = 0.00001;
	areEqual(output_C,output_asm,length,tol,"Kalman filter test failed");
	
	float pDst_C[length];
	float pDst_CMSIS[length];
	
	// Array Difference
	subs_diff(pIn, pOut, pDst_C, length);
	arm_sub_f32 ( pIn, pOut, pDst_CMSIS, length);
	
	areEqual(pDst_C,pDst_CMSIS,length,tol,"Difference test failed");
	
	// Array Mean
	float mean_C;
	float32_t mean_CMSIS;
	average(pDst_C, length, &mean_C);
	arm_mean_f32(pDst_CMSIS, length, &mean_CMSIS);
	
	printf("%f\t%f\n", mean_C, mean_CMSIS);
	
	// Standard Deviation
	float std_C;
	float32_t std_CMSIS;
	standard_deviation(pDst_C, length, &std_C);
	arm_std_f32(pDst_CMSIS, length, &std_CMSIS);
	
	printf("%f\t%f\n", std_C, std_CMSIS);
	
	float pFilter_C[2*length-1];
	float pFilter_CMSIS[2*length-1];
	
	// Correlation
	correlation(pIn, length, pOut, length, pFilter_C);
	arm_correlate_f32 (pIn, length, pOut, length, pFilter_CMSIS);

	areEqual(pFilter_C,pFilter_CMSIS,length,tol,"Correlation test failed");

	// Convolution
	convolution(pIn, length, pOut, length, pFilter_C);
	arm_conv_f32 (pIn, length, pOut, length, pFilter_CMSIS);
	
	areEqual(pFilter_C,pFilter_CMSIS,length,tol,"Convolution filter test failed");
	
	printf("Program End\n");
	
	while(1);
	return 0;
}



