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

Kalmanfilter_asm

	;LOAD PARAMETERS FROM STRUCT ==============
	VLDR.32 S0, [R2]	;load q state
	VLDR.32 S1, [R2,#4]	;load r state
	VLDR.32 S2, [R2,#8]	;load x state
	VLDR.32 S3, [R2,#12];load p state
	 
	;LOOP =====================================
loop
	
	VADD.F32 S3, S3, S0 ;p = p + q
	VADD.F32 S5, S3, S1	;temp = p + r
	VDIV.F32 S4, S3, S5	;k = p / p + r

	VLDR.32 S5, [R0]	;load next measurement from input array
	ADD R0, R0, #4		;increment input array index

	VSUB.F32 S5, S5, S2	;temp = measurement - x
	VMUL.F32 S5, S4, S5	;temp = k * (measurement - x)
	VADD.F32 S2, S2, S5	;x = x + k * (measurement - x)

	VLDR.F32 S5, =1.0 	
	VSUB.F32 S5, S5, S4	;temp = 1 - k
	VMUL.F32 S3, S5, S3 ;p = (1 - k) * p

	VSTR.32 S2, [R1]	;store x value to output array
	ADD R1, R1, #4		;increment output array index

	SUBS R3, R3, #1		;decrement array length (# of elements remaining)
	
	BEQ	store
	
	VADD.F32 S3, S3, S0 ;p = p + q
	VADD.F32 S5, S3, S1	;temp = p + r
	VDIV.F32 S4, S3, S5	;k = p / p + r

	VLDR.32 S5, [R0]	;load next measurement from input array
	ADD R0, R0, #4		;increment input array index

	VSUB.F32 S5, S5, S2	;temp = measurement - x
	VMUL.F32 S5, S4, S5	;temp = k * (measurement - x)
	VADD.F32 S2, S2, S5	;x = x + k * (measurement - x)

	VLDR.F32 S5, =1.0 	
	VSUB.F32 S5, S5, S4	;temp = 1 - k
	VMUL.F32 S3, S5, S3 ;p = (1 - k) * p

	VSTR.32 S2, [R1]	;store x value to output array
	ADD R1, R1, #4		;increment output array index

	SUBS R3, R3, #1		;decrement array length (# of elements remaining)
	
	BEQ	store
	
	VADD.F32 S3, S3, S0 ;p = p + q
	VADD.F32 S5, S3, S1	;temp = p + r
	VDIV.F32 S4, S3, S5	;k = p / p + r

	VLDR.32 S5, [R0]	;load next measurement from input array
	ADD R0, R0, #4		;increment input array index

	VSUB.F32 S5, S5, S2	;temp = measurement - x
	VMUL.F32 S5, S4, S5	;temp = k * (measurement - x)
	VADD.F32 S2, S2, S5	;x = x + k * (measurement - x)

	VLDR.F32 S5, =1.0 	
	VSUB.F32 S5, S5, S4	;temp = 1 - k
	VMUL.F32 S3, S5, S3 ;p = (1 - k) * p

	VSTR.32 S2, [R1]	;store x value to output array
	ADD R1, R1, #4		;increment output array index

	SUBS R3, R3, #1		;decrement array length (# of elements remaining)
	
	BNE  loop			;if end of array not reached, loop

	;LOOP END=================================

store
	;STORE PARAMETERS TO STRUCT ==============
	VSTR.32 S0, [R2]	;store q
	VSTR.32 S1, [R2,#4]	;store r
	VSTR.32 S2, [R2,#8]	;store x
	VSTR.32 S3, [R2,#12];store p
	VSTR.32 S4, [R2,#16];store k

	BX LR

	END



