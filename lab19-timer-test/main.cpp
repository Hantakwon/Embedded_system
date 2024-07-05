#include "mbed.h"

#define GREEN_LED_PIN PA_13
#define YELLOW_LED_PIN PB_10

DigitalOut greenLed(GREEN_LED_PIN);
DigitalOut yellowLed(YELLOW_LED_PIN);

Ticker fastTicker;
Ticker slowTicker;
Timeout stopTimeout;

void toggleGreenLed(){
	greenLed = !greenLed;
}

void toggleYellowLed() {
	yellowLed = !yellowLed;
} 

void stopLedToggle(){
	fastTicker.detach();
	slowTicker.detach();
}

void setup(){
	fastTicker.attach(&toggleGreenLed,0.2);
	slowTicker.attach(&toggleYellowLed,1);
	stopTimeout.attach(&stopLedToggle,10);
}

int main() {
		setup();
	
    while(1) {
			wait_ms(2000);
		}
}
