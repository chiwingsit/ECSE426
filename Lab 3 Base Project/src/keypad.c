#include "keypad.h"


char keys[4][3] = {  
 {'1', '2', '3'},
 {'4', '5', '6'},
 {'7', '8', '9'},
 {'*', '0', '#'}
};


int get_column()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = ROW_0 | ROW_1 | ROW_2 | ROW_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = COL_0 | COL_1 | COL_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	if(GPIO_ReadInputDataBit(GPIOB, COL_0) == 0)
		return 0;
	else if(GPIO_ReadInputDataBit(GPIOB, COL_1) == 0)
		return 1;
	else if(GPIO_ReadInputDataBit(GPIOB, COL_2) == 0)
		return 2;
	else
		return -1;
}

int get_row()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = COL_0 | COL_1 | COL_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = ROW_0 | ROW_1 | ROW_2 | ROW_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	if(GPIO_ReadInputDataBit(GPIOD, ROW_0) == 0)
		return 0;
	else if(GPIO_ReadInputDataBit(GPIOD, ROW_1) == 0)
		return 1;
	else if(GPIO_ReadInputDataBit(GPIOD, ROW_2) == 0)
		return 2;
	else if(GPIO_ReadInputDataBit(GPIOD, ROW_3) == 0)
		return 3;
	else
		return -1;
}

char get_key()
{
	int column = get_column();
	int row = get_row();
		
	if(column == -1 || row == -1)
	{
		return '\0';
	}
	else
	{
		return keys[row][column];
	}
}


