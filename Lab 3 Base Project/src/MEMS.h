#ifndef MEMS_h
#define MEMS_h

#include <stdio.h>
#include "lis3dsh.h"
#include "KalmanFilter.h"
#include "math.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

extern double x_data;
extern double y_data;
extern double z_data;

void MEMS_config();
void MEMS_read_value();
void MEMS_interrupt_config();
void EXTI1_IRQHandler();

#endif
