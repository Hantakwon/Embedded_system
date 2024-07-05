#ifndef _LIGHT_CONTROL_H_
#define _LIGHT_CONTROL_H_

#include "mbed.h"

#define GREEN_LED 	PA_13
#define YELLOW_LED 	PB_10
#define RED_LED 		PA_4
#define RGB_GREEN		PC_6
#define RGB_BLUE		PB_0
#define RGB_RED			PA_1

typedef enum { 
	GREEN=0x04, 
	YELLOW = 0x02, 
	RED = 0x01
} light_color_t;

BusOut lightA(RED_LED, YELLOW_LED,GREEN_LED);
#define LIGHT_A_GREEN 	0x04
#define LIGHT_A_YELLOW	0x02
#define LIGHT_A_RED 		0x01
BusOut lightB(RGB_RED, RGB_BLUE, RGB_GREEN);
#define LIGHT_B_GREEN 	0x04
#define LIGHT_B_YELLOW	0x05
#define LIGHT_B_RED 		0x01

void controlLightA(light_color_t color){
	lightA = color;
}

void controlLightB(light_color_t color){
	if (color == YELLOW){
		lightB = LIGHT_B_YELLOW;
	}
	else {
		lightB = color;
	}
}
#endif