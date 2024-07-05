#include "mbed.h"

#define GREEN_LED_FIN PA_13
#define YELLOW_LED_FIN PB_10
#define RED_LED_FIN PA_4

#define FIRST_BTN PA_14

DigitalOut greenLed(GREEN_LED_FIN);
DigitalOut yellowLed(YELLOW_LED_FIN);
DigitalOut redLed(RED_LED_FIN);

DigitalIn firstBtn(FIRST_BTN);

Timer redLedBlinkTmr;
Timer yellowLedBlinkTmr;

typedef enum {
	NO_EDGE, RISING_EDGE, FALLING_EDGE
} edge_t;

edge_t detectFirstBtnEdge() {
	static int prevValue = 1;
	
	int currValue = firstBtn.read();
	if (currValue != prevValue){
		wait_ms(50);
		prevValue = currValue;
		return (currValue == 1) ? RISING_EDGE : FALLING_EDGE;
	}
	return NO_EDGE;
}

void toggleRedLed() {
	if (redLedBlinkTmr.read() >= 2.0) {
		redLed = !redLed;
		redLedBlinkTmr.reset();
	}
}

void toggleYellowLed() {
	if (yellowLedBlinkTmr.read() >= 1.0) {
		yellowLed = !yellowLed;
		yellowLedBlinkTmr.reset();
	}
}

void setup(){
	greenLed = 0;
	redLedBlinkTmr.start();
	yellowLedBlinkTmr.start();
}

int main() {

	setup();
	
    while(1) {
			if (detectFirstBtnEdge() == FALLING_EDGE) {
				greenLed = !greenLed;
				wait(0.5);
			}
			toggleRedLed();
			toggleYellowLed();
    }
	}

