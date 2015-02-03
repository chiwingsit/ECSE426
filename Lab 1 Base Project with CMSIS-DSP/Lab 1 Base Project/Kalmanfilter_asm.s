	AREA text, CODE, READONLY
	EXPORT Kalmanfilter_asm
	
	; R0: pointer to input array
	; R1: pointer to output array
	; R2: pointer to struct
	; R3: array length

	; S0: value of q
	; S1: value of r
	; S2: value of x
	; S3: value of p
	; S4: value of k
	; S5: value of next measurement from input
	; S6: temp
	
test_workbench

	;create arrays
input 	DCFS 	0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0
output 	DCFS 	0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0
	;set pointer to arrays
	ADR R0, input
	ADR R1, output
	;initialise values
	;q
	VLDR.F32 S0, =0.1
	;r
	VLDR.F32 S1, =0.1
	;x
	VLDR.F32 S2, =0.0
	;p
	VLDR.F32 S3, =0.1
	
	B loop

Kalmanfilter_asm


	;LOAD PARAMETERS FROM STRUCT ==============
	;load q state
	VLDR.32 S0, [R2]
	;load r state
	VLDR.32 S1, [R2,#4]
	;load x state
	VLDR.32 S2, [R2,#8]
	;load p state
	VLDR.32 S3, [R4,#12]
	 
	;LOOP =====================================
loop

	 ;load next measurement from input array
	 VLDR.32 S5, [R0]
	 ;increment input array index
	 ADD R0, R0, #4

	 ;p = p + q
	 VADD.F32 S3, S3, S0
	 
	 ;temp = p + r
	 VADD.F32 S6, S3, S1
	 ;k = p / p + r
	 VDIV.F32 S4, S3, S6

	 ;temp = measurement - x
	 VSUB.F32 S6, S5, S2
	 ;temp = k * (measurement - x)
	 VMUL.F32 S6, S4, S6
	 ;x = x + k * (measurement - x)
	 VADD.F32 S2, S2, S6
	 
	 ;temp = 1 - k
	 VLDR.F32 S7, =1.0
	 VSUB.F32 S6, S7, S4
	 ;p = (1 - k) * p
	 VMUL.F32 S3, S6, S3
	 
	 ;store x value to output array
	 VSTR.32 S2, [R1]
	 ;increment output array index
	 ADD R1, R1, #4
	 
	 ;decrement array length (# of elements remaining)
	 SUBS R3, R3, #1
	 ;if end of array not reached, loop
	 BNE  loop
	 
	 ;LOOP END=================================
	 
	 ;STORE PARAMETERS TO STRUCT ==============
	 ;store q
	 VSTR.32 S0, [R2]
	 ;store r
	 VSTR.32 S1, [R2,#4]
	 ;store x
	 VSTR.32 S2, [R4,#8]
	 ;store p
	 VSTR.32 S3, [R4,#12]
	 ;store k
	 VSTR.32 S4, [R4,#16]
	 
	 BX LR
	 
	 END
	 
	 
	 
	  