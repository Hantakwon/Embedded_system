#include "mbed.h"
#include "button_input.h"
#include "light_control.h"

enum { A, AtoB, B, BtoA } state;

#define PASS_WAIT_SEC 10
#define TRANS_WAIT_SEC 3

Ticker waitTicker;
volatile int waitTime = 0;

void waitTickerHandler() {
	if ( waitTime > 0) --waitTime;
}
void setup(){
	waitTicker.attach(&waitTickerHandler, 1.0);
	state = A;
	waitTime = PASS_WAIT_SEC;
}
int main() {
	
		setup();
	
    while(1) {
			switch (state) {
				case A :
					controlLightA(GREEN);
					controlLightB(RED);
					if (waitTime == 0 || emgButtonPressed()) {
						waitTime = TRANS_WAIT_SEC;
						state = AtoB;
					}
					break;
				case AtoB :
					controlLightA(YELLOW);
					if (waitTime == 0 || emgButtonPressed()) {
						waitTime = PASS_WAIT_SEC;
						state = B;
					}
					break;
				case B :
					controlLightA(RED);
					controlLightB(GREEN);
					if (waitTime == 0 || emgButtonPressed()) {
						waitTime = TRANS_WAIT_SEC;
						state = BtoA;
					}
					break;
				case BtoA :
					controlLightB(YELLOW);
					if (waitTime == 0 || emgButtonPressed()) {
						waitTime = PASS_WAIT_SEC;
						state = A;
					}
				default :
					break;
			}
		}		
}
