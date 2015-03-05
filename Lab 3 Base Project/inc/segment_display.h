#ifndef segment_display_h
#define segment_display_h

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

// GPIO PORT B
#define SELECT_0 GPIO_Pin_4
#define SELECT_1 GPIO_Pin_5
#define SELECT_2 GPIO_Pin_7
#define SELECT_3 GPIO_Pin_8

// GPIO PORT E
#define SEGMENT_A  GPIO_Pin_7
#define SEGMENT_B  GPIO_Pin_8
#define SEGMENT_C  GPIO_Pin_9
#define SEGMENT_D  GPIO_Pin_10
#define SEGMENT_E  GPIO_Pin_11
#define SEGMENT_F  GPIO_Pin_12
#define SEGMENT_G  GPIO_Pin_13
#define SEGMENT_DP GPIO_Pin_14

void seg_disp_init();
void display_digit(uint8_t digit);
void enable_pos(uint8_t digit);
void test_display();

#endif


