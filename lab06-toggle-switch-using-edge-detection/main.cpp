#include "mbed.h"

#define GREEN_LED_FIN PA_13
#define FIRST_BTN PA_14

DigitalOut greenLed(GREEN_LED_FIN);
DigitalIn firstBtn(FIRST_BTN);

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

void setup(){
	greenLed = 0;
}

int main() {
    while(1) {
			if (detectFirstBtnEdge() == FALLING_EDGE) {
				greenLed = !greenLed;
				wait(0.5);
    }
	}
}
