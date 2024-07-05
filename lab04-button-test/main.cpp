#include "mbed.h"

#define GREEN_LED PA_13
#define FIRST_BTN PA_14

DigitalOut greenLed(GREEN_LED);
DigitalIn firstBtn(FIRST_BTN);

void setup() {
	greenLed = 0;
}
int main() {
		setup();
    while(1) {
			greenLed = !firstBtn;
    }
		
}