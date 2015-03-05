#include "MEMS.h"

double x_data;
double y_data;
double z_data;

double pitch;
double roll;

// calibration offset values
static const double X_OFFSET = 15.51869983;
static const double Y_OFFSET = -50.09611393;
static const double Z_OFFSET = -4.045411972;

static const double PI = 3.14159265359;

// initialize Kalman filter values
kalman_state x_kstate = {
	0.001,         // q
	3.821383247,	 // r
	1048,          // x
	1,             // p
	0              // k
};

kalman_state y_kstate = {
	0.001,         // q
	3.821383247,	 // r
	1048,          // x
	1,             // p
	0              // k
};

kalman_state z_kstate = {
	0.001,         // q
	3.821383247,	 // r
	1048,          // x
	1,             // p
	0              // k
};



// configures LIS3DSH settings and initializes it
void MEMS_config (){
	
	LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
	
	// configure the LIS3DSH parameters
	LIS3DSH_InitStruct.AA_Filter_BW 								= LIS3DSH_AA_BW_800;
	LIS3DSH_InitStruct.Axes_Enable 									= LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;
	LIS3DSH_InitStruct.Continous_Update 						= LIS3DSH_ContinousUpdate_Disabled;
	LIS3DSH_InitStruct.Full_Scale 									= LIS3DSH_FULLSCALE_2;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate 	= LIS3DSH_DATARATE_100;
	LIS3DSH_InitStruct.Self_Test 										= LIS3DSH_SELFTEST_NORMAL;

	// initialize LIS3DSH
	LIS3DSH_Init(&LIS3DSH_InitStruct);
}



/* reads X,Y,Z values from sensor
   filters values using Kalman */
void MEMS_read_value () {
	//Stores values obtained from sensor
	float xyz_data[3];
	
	while (1){
		// read values from ACC and store in xyz_data array
		LIS3DSH_ReadACC(xyz_data);
		
		x_data = (double)xyz_data[0];
		y_data = (double)xyz_data[1];
		z_data = (double)xyz_data[2];
		
		// use offset to calibrate values
		x_data -= X_OFFSET;
		y_data -= Y_OFFSET;
		z_data -= Z_OFFSET;	
		
		// use Least Square method to calibrate values 
		//x_data = 0.9645*x_data -0.0136*y_data + 0.0766*z_data -14.9427;
		//y_data = 0.063*x_data + 0.9893*y_data + 0.0097*z_data -16.1191;
		//z_data = 0.0104*x_data + 0.0320*y_data + 0.9680*z_data + 10.7952;
		
		printf ("X: %f, Y: %f, Z: %f\n", x_data, y_data, z_data);
		//printf ("%f, %f, %f\n", x_data, y_data, z_data);
		//printf ("%f\n", y_data);
		
		// filter X, Y, Z using Kalman
		Kalmanfilter_C(x_data, &x_kstate);
		x_data = x_kstate.x;
		Kalmanfilter_C(y_data, &y_kstate);
		y_data = y_kstate.x;
		Kalmanfilter_C(z_data, &z_kstate);
		z_data = z_kstate.x;
		
		//Tri-axis tilt sensing method for calculation pitch and roll
		pitch = ((180.0/PI)*atan(x_data/(sqrt((y_data*y_data)+(z_data*z_data)))));
		roll = ((180.0/PI)*atan(y_data/(sqrt((x_data*x_data)+(z_data*z_data)))));
		
	}
}

// configures settings for interrupt TODO: test
void MEMS_interrupt_config() {
	
	EXTI_InitTypeDef exti_init_s;
	NVIC_InitTypeDef nvic_init_s;
	GPIO_InitTypeDef gpio_init_s;

	uint8_t ctrl;
	ctrl = 0x20;
	LIS3DSH_Write(&ctrl, LIS3DSH_CTRL_REG4, 1);
	// Enable GPIOE clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//	Enable SYSCFG clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // enable external interrupts 
	
	//	Configure GPIOE PE1 as floating
	gpio_init_s.GPIO_Pin = GPIO_Pin_1;
	gpio_init_s.GPIO_Mode = GPIO_Mode_IN; // Set as input
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz; // Don't limit slew rate
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL; // external pull-ups 
	GPIO_Init(GPIOE, &gpio_init_s);
	
	
	//	Connect PE1 pin to EXTI Line 1 (INT2)
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
	// Configure EXTI Line 1 
	exti_init_s.EXTI_Line = EXTI_Line1;
	exti_init_s.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_s.EXTI_Trigger = EXTI_Trigger_Rising;  
	exti_init_s.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_init_s);
	
	//	Enable and set EXTI Line1 Interrupt to the lowest priority
	nvic_init_s.NVIC_IRQChannel = EXTI1_IRQn;
	nvic_init_s.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_init_s.NVIC_IRQChannelSubPriority = 0;
	nvic_init_s.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init_s);
}

