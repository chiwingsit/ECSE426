#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "MEMS.h"


int main(){
	
	while(1){
	MEMS_config();
	MEMS_read_value();
	}
	
	return 0;
}

