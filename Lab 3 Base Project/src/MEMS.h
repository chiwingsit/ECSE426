#ifndef MEMS_h
#define MEMS_h

#include <stdio.h>
#include "lis3dsh.h"
#include "KalmanFilter.h"
#include "math.h"

void MEMS_config();
void MEMS_interrupt_config();

#endif