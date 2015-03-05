#ifndef keypad_h
#define keypad_h

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

// GPIO PORT B
#define COL_0 GPIO_Pin_11
#define COL_1 GPIO_Pin_12
#define COL_2 GPIO_Pin_13

// GPIO PORT D
#define ROW_0 GPIO_Pin_8
#define ROW_1 GPIO_Pin_9
#define ROW_2 GPIO_Pin_10
#define ROW_3 GPIO_Pin_11

char get_key();

#endif


