	AREA text, CODE, READONLY
	IMPORT Kalmanfilter_asm
	IMPORT output
	IMPORT struct
	EXPORT test_workbench

input	DCFS	0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0 	;create input array

test_workbench

	;INITIALIZE PARAMETERS ==============
	ADR R0, input	;set pointer to input arrays
	LDR R1, =output	;set pointer to output arrays
	LDR R2, =struct	;set pointer to struct
	LDR R3, =10 	;set array length
	LDR R5, =output

	VLDR.F32 S0, =0.1
	VSTR.32 S0, [R2]	;initialise q
	VSTR.32 S0, [R2,#4]	;initialise r
	VSTR.32 S0, [R2,#12];initialise p
	VLDR.F32 S0, =0.0
	VSTR.32 S0, [R2,#8]	;initialise x

	;BRANCH TO KALMANFILTER =============
	LDR     R4, =Kalmanfilter_asm
	BX      R4
	
	END
