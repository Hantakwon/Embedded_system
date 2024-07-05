#include "mbed.h"
#include "light_control.h"

#define GREEN_LED 	PA_13
#define YELLOW_LED 	PB_10
#define RED_LED 		PA_4
/*
#define GREEN_RGB		PC_6
#define BLUE_RGB		PB_0
#define RED_RGB			PA_1
*/
DigitalOut greenLed(GREEN_LED);
DigitalOut yellowLed(YELLOW_LED);
DigitalOut redLed(RED_LED);

static Timeout offTimeout;

void LEDOff(){
	greenLed = 0;
	redLed = 0;
};

void activeLEDOnCorrect(){
	greenLed = 1;
	offTimeout.attach(LEDOff, 3.0);
}

void activeLEDOnIncorrect(){
	redLed = 1;
	offTimeout.attach(LEDOff, 3.0);
}