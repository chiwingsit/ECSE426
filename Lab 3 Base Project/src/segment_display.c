#include "segment_display.h"

unsigned int ticks = 0; //for timer
STATE keypad_state;

void seg_disp_init()
{
	timer_init();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//	Configure GPIOB for the 4 select lines
	GPIO_InitStruct.GPIO_Pin = SELECT_1 | SELECT_2 | SELECT_3 | SELECT_4;
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
	
	GPIO_SetBits(GPIOB, SELECT_1);
	GPIO_SetBits(GPIOB, SELECT_2);
	GPIO_SetBits(GPIOB, SELECT_3);
	GPIO_SetBits(GPIOB, SELECT_4);
}

void display_digit(uint8_t digit, uint8_t display_point)
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

	if(display_point){
		SEG_DP = Bit_SET;
	}
	
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
			SEG_F = Bit_SET;
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
			SEG_E = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 5:
			SEG_A = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
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
		BitAction pos1 = Bit_RESET;
		BitAction pos2 = Bit_RESET;
		BitAction pos3 = Bit_RESET;
		BitAction pos4 = Bit_RESET;
	
		switch(pos){
			case 1:
				pos1 = Bit_SET;
				break;
			case 2:
				pos2 = Bit_SET;
				break;
			case 3:
				pos3 = Bit_SET;
				break;
			case 4:
				pos4 = Bit_SET;
				break;
		}

		// write corresponding bits to LEDs
		GPIO_WriteBit(GPIOB, SELECT_1, pos1);
		GPIO_WriteBit(GPIOB, SELECT_2, pos2);
		GPIO_WriteBit(GPIOB, SELECT_3, pos3);
		GPIO_WriteBit(GPIOB, SELECT_4, pos4);
}

void display_angle(float angle)
{
	int digit;
	int digit_pos = (ticks % 3) + 1;
	
	if(angle >= 100){
		digit = (int) (angle / pow(10, 3-digit_pos)) % 10;
		enable_pos(digit_pos);
		display_digit(digit,0);
	}
	else if(angle >= 10)
	{
		digit = (int) (angle / pow(10, 2-digit_pos)) % 10;
		enable_pos(digit_pos);
		if(digit_pos == 2){
			display_digit(digit,1);
		}
		else{
			display_digit(digit,0);
		}
	}
	else{
		digit = (int) (angle / pow(10, 1-digit_pos)) % 10;
		enable_pos(digit_pos);
		if(digit_pos == 1){
			display_digit(digit,1);
		}
		else{
			display_digit(digit,0);
		}
	}
		
}

void display_high()
{
	int digit_pos = (ticks % 4) + 1;
	enable_pos(digit_pos);
		// reset all segments
	BitAction SEG_A  = Bit_RESET;
	BitAction SEG_B  = Bit_RESET;
	BitAction SEG_C  = Bit_RESET;
	BitAction SEG_D  = Bit_RESET;
	BitAction SEG_E  = Bit_RESET;
	BitAction SEG_F  = Bit_RESET;
	BitAction SEG_G  = Bit_RESET;

	switch(digit_pos){
		case 1:
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_E = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 2:
			SEG_F = Bit_SET;
			break;
		case 3:
			SEG_A = Bit_SET;
			SEG_B = Bit_SET;
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 4:
			SEG_C = Bit_SET;
			SEG_E = Bit_SET;
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
}

void display_low()
{
	int digit_pos = (ticks % 4) + 1;
	enable_pos(digit_pos);
	// reset all segments
	BitAction SEG_A  = Bit_RESET;
	BitAction SEG_B  = Bit_RESET;
	BitAction SEG_C  = Bit_RESET;
	BitAction SEG_D  = Bit_RESET;
	BitAction SEG_E  = Bit_RESET;
	BitAction SEG_F  = Bit_RESET;
	BitAction SEG_G  = Bit_RESET;

	switch(digit_pos){
		case 1:
			SEG_D = Bit_SET;
			SEG_E = Bit_SET;
			SEG_F = Bit_SET;
			break;
		case 2:
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_F = Bit_SET;
			SEG_G = Bit_SET;
			break;
		case 3:
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_F = Bit_SET;
			break;
		case 4:
			SEG_C = Bit_SET;
			SEG_D = Bit_SET;
			SEG_F = Bit_SET;
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
}

void display_int(int n)
{
	int digit;
	int digit_pos = (ticks % 4) + 1;
	enable_pos(digit_pos);
	digit = (int) (n / pow(10, 4-digit_pos)) % 10;
	display_digit(digit,0);
}

int display_keypad_input()
{
	timer_init();
	int input = 0;
	
	char c = get_key();
	while(c != '#')
	{
		display_int(input);
		if(input < 1000){
			if(c != NO_KEY){
				input = (input * 10) + c - '0';
			}
		}
		c = get_key();
	}
	
	return input;
}

void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		ticks++;
		if(ticks == 5){
			ticks = 0;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

