#include "segment_display.h"

void seg_disp_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//	Configure GPIOB for the 4 select lines
	GPIO_InitStruct.GPIO_Pin = SELECT_0 | SELECT_1 | SELECT_2 | SELECT_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//	Configure GPIOE for the segments
	GPIO_InitStruct.GPIO_Pin = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G | SEGMENT_DP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void test_display()
{
	GPIO_SetBits(GPIOE, SEGMENT_A);
	GPIO_SetBits(GPIOE, SEGMENT_B);
	GPIO_SetBits(GPIOE, SEGMENT_C);
	GPIO_SetBits(GPIOE, SEGMENT_D);
	GPIO_SetBits(GPIOE, SEGMENT_E);
	GPIO_SetBits(GPIOE, SEGMENT_F);
	GPIO_SetBits(GPIOE, SEGMENT_G);
	GPIO_SetBits(GPIOE, SEGMENT_DP);
	
	GPIO_SetBits(GPIOB, SELECT_0);
	GPIO_SetBits(GPIOB, SELECT_1);
	GPIO_SetBits(GPIOB, SELECT_2);
	GPIO_SetBits(GPIOB, SELECT_3);
}

void display_symbol(uint8_t digit)
{
	// reset all segments
	BitAction SEG_A  = Bit_RESET;
	BitAction SEG_B  = Bit_RESET;
	BitAction SEG_C  = Bit_RESET;
	BitAction SEG_D  = Bit_RESET;
	BitAction SEG_E  = Bit_RESET;
	BitAction SEG_F  = Bit_RESET;
	BitAction SEG_G  = Bit_RESET;
	BitAction SEG_DP = Bit_RESET;

	switch(digit){
		case 0:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
			SEG_F = Bit_SET;
			break;
		case 1:
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			break;
		case 2:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 3:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 4:
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 5:
			SEG_A = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 6:
			SEG_A = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 7:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			break;
		case 8:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 9:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
	}

	// write corresponding bits to the pin
	GPIO_WriteBit(GPIOE, SEGMENT_A, SEG_A);
	GPIO_WriteBit(GPIOE, SEGMENT_B, SEG_B);
	GPIO_WriteBit(GPIOE, SEGMENT_C, SEG_C);
	GPIO_WriteBit(GPIOE, SEGMENT_D, SEG_D);
	GPIO_WriteBit(GPIOE, SEGMENT_E, SEG_E);
	GPIO_WriteBit(GPIOE, SEGMENT_F, SEG_F);
	GPIO_WriteBit(GPIOE, SEGMENT_G, SEG_G);
	GPIO_WriteBit(GPIOE, SEGMENT_DP, SEG_DP);
}

void enable_pos(uint8_t pos) 
{
		// reset all LEDs
		BitAction pos0 = Bit_RESET;
		BitAction pos1 = Bit_RESET;
		BitAction pos2 = Bit_RESET;
		BitAction pos3 = Bit_RESET;
	
		switch(pos){
			case 0:
				pos0 = Bit_SET;
				break;
			case 1:
				pos1 = Bit_SET;
				break;
			case 2:
				pos2 = Bit_SET;
				break;
			case 3:
				pos3 = Bit_SET;
				break;
		}

		// write corresponding bits to LEDs
		GPIO_WriteBit(GPIOB, SELECT_0, pos0);
		GPIO_WriteBit(GPIOB, SELECT_1, pos1);
		GPIO_WriteBit(GPIOB, SELECT_2, pos2);
		GPIO_WriteBit(GPIOB, SELECT_3, pos3);
}


