#include "mbed.h"

#define GREEN_LED_FIN PA_13
#define YELLOW_LED_FIN PB_10
#define RED_LED_FIN PA_4

DigitalOut greenLed(GREEN_LED_FIN);
DigitalOut yellowLed(YELLOW_LED_FIN);
DigitalOut redLed(RED_LED_FIN);

void setup(){
		greenLed = 0;
		yellowLed = 0;
		redLed = 0;
}

int main() {
	
		setup();
	
    while(1) {
        greenLed = !greenLed; // LED is ON
        wait(0.5); // 200 ms
				greenLed = !greenLed; // LED is
        wait(0.5); // 200 ms
				redLed = !redLed; // LED is ON
        wait(0.5); // 200 ms
				redLed = !redLed; // LED is
        wait(0.5); // 200 ms
				yellowLed = !yellowLed; // LED is ON
        wait(0.5); // 200 ms
				yellowLed = !yellowLed; // LED is
        wait(0.5); // 200 ms
    }
		
}
