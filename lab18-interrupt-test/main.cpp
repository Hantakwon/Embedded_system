#include "mbed.h"

#define GREEN_LED_PIN PA_13
#define FIRST_BTN_PIN PA_14

DigitalOut greenLed(GREEN_LED_PIN);
InterruptIn firstBtn(FIRST_BTN_PIN);

void firstBtnIrqHandler() {
	greenLed = !greenLed;
}

void setup(){
	firstBtn.fall(&firstBtnIrqHandler);
}

int main() {
	
		setup();
	
    while(1) {		
			wait_ms(2000);
    }
}
