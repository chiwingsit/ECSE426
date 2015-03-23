#ifndef segment_display_h
#define segment_display_h

#include <math.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_conf.h"
#include "keypad.h"
#include "cmsis_os.h"
#include "timer.h"

// GPIO PORT B
#define SELECT_1 GPIO_Pin_4
#define SELECT_2 GPIO_Pin_5
#define SELECT_3 GPIO_Pin_7
#define SELECT_4 GPIO_Pin_8

// GPIO PORT E
#define SEGMENT_A  GPIO_Pin_7
#define SEGMENT_B  GPIO_Pin_8
#define SEGMENT_C  GPIO_Pin_9
#define SEGMENT_D  GPIO_Pin_10
#define SEGMENT_E  GPIO_Pin_6
#define SEGMENT_F  GPIO_Pin_12
#define SEGMENT_G  GPIO_Pin_13
#define SEGMENT_DP GPIO_Pin_14
#define SEGMENT_DT GPIO_Pin_15

void seg_disp_init();
void display_digit(uint8_t digit, uint8_t display_point);
void enable_pos(uint8_t pos);
void display_angle(float angle);
void display_int(uint32_t n);
int display_keypad_input();
void display_high();
void display_low();
void test_display();
void disable_display();

#endif


