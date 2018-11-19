#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "PODS.h"
#include "Arduino.h"
#include "constants.h"
#include "driver/ledc.h"
#include "Servo_Control.hpp"


////////////////////////////////////////////////////////////////////////////////
//                               BEGIN CODE HERE                              //
////////////////////////////////////////////////////////////////////////////////

void startPOD(bool start, int x)
{
	
	//x = wifi.read();

	switch(x)
	{
		case 0: gyger0_start = start;
			break;
		case 1: gyger1_start = start;
			break;
		case 2: gyger2_start = start;
			break;
		case 3: gyger3_start = start;
			break;
		case 4: gyger4_start = start;
			break;
		case 5: gyger5_start = start;
			break;
		case 6: gyger6_start = start;
			break;
		default:
			break;
	}
}


void sealPODS(int x)
{

uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin = servoHatchPin(x);
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 5;
float servo1_max = 10;

		//servo object for PODS door
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	servo1.SetPositionPercent(getPercent(-90));


}

/**************************
	rotates a servo to despens an amount of fluid
****************************/
void dispenseFluid(int x)
{

uint32_t servo1_frequency = 50; //hz
uint32_t servo1_gpio_pin = servoInoculationPin(x);
uint32_t servo1_timer = 0;
uint32_t servo1_channel = 0;
float servo1_min = 5;
float servo1_max = 10;


		//servo object for sterilization fluid
	Servo servo2(inoculation_servo7_pin,servo1_channel,servo1_timer, 
					servo1_frequency, servo1_max, servo1_min);

		//servo object for inoculation fluid
	Servo servo1(servo1_gpio_pin,servo1_channel,servo1_timer, 
		servo1_frequency, servo1_max, servo1_min);

	servo1.SetPositionPercent(getPercent(-90));

	if(x == 7)
	{
		servo2.SetPositionPercent(getPercent(-90));
	}

	vTaskDelay(5000 / portTICK_PERIOD_MS);

	if(x == 7)
	{
		servo2.SetPositionPercent(getPercent(90));
	}

	servo1.SetPositionPercent(getPercent(90));

}


int getPercent(int angle)
{
	//servo 5-10% 
	int percent = map(angle, -90, 90, 0, 100);

	if(percent < 5 or percent > 10)
	{
		percent = 7.5;
	}


	return percent;

}




int servoInoculationPin(int x)
{
			switch(x)
	{
		case 0: return inoculation_servo0_pin;
			break;
		case 1: return inoculation_servo1_pin;
			break;
		case 2: return inoculation_servo2_pin;
			break;
		case 3: return inoculation_servo3_pin;
			break;
		case 4: return inoculation_servo4_pin;
			break;
		case 5: return inoculation_servo5_pin;
			break;
		case 6: return inoculation_servo6_pin;
			break;
		default: return -1;
			break;
	}
}

int servoHatchPin(int x)
{
			switch(x)
	{
		case 0: return hatch_servo_pin0;
			break;
		case 1: return hatch_servo_pin1;
			break;
		case 2: return hatch_servo_pin2;
			break;
		case 3: return hatch_servo_pin3;
			break;
		case 4: return hatch_servo_pin4;
			break;
		case 5: return hatch_servo_pin5;
			break;
		case 6: return hatch_servo_pin6;
			break;
		default: return -1;
			break;
		}
}

/*********************************
	interupt functions
********************************/

void initInteruptPins()
{

	pinMode(gyger0_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger0_pin), emissionCount0, FALLING);

	pinMode(gyger1_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger1_pin), emissionCount1, FALLING);
	
	pinMode(gyger2_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger2_pin), emissionCount2, FALLING);
	
	pinMode(gyger3_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger3_pin), emissionCount3, FALLING);
	
	pinMode(gyger4_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger4_pin), emissionCount4, FALLING);
	
	pinMode(gyger5_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger5_pin), emissionCount5, FALLING);
	
	pinMode(gyger6_pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(gyger6_pin), emissionCount6, FALLING);
	

}

void emissionCount0()
{
	eCount0++;
}

void emissionCount1()
{
	eCount1++;
}

void emissionCount2()
{
	eCount2++;
}
void emissionCount3()
{
	eCount3++;
}
void emissionCount4()
{
	eCount4++;
}
void emissionCount5()
{
	eCount5++;
}
void emissionCount6()
{
	eCount6++;
}
/////////////////////////////////////////////////////////////////////////////////                               CODE ENDS HERE                               //
////////////////////////////////////////////////////////////////////////////////
