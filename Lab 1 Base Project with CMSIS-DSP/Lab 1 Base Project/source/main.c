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
	
	return 0;
}




