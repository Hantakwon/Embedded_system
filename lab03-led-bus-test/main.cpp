#include "mbed.h"

#define GREEN_LED_FIN PA_13
#define YELLOW_LED_FIN PB_10
#define RED_LED_FIN PA_4

BusOut ledBus(RED_LED_FIN, YELLOW_LED_FIN, GREEN_LED_FIN);

void setup(){
		ledBus = 0;
}

int counter = 0 ;

int main() {
	
	setup();
	
  while(1) {
		ledBus = counter;
		wait(0.5);
		counter = ++counter % 8;
	}
		
}

/*
int main() {
	
		setup();
	
    while(1) {
			ledBus = GREEN_LED_ON;
			wait(0.5);
			ledBus = YELLOW_LED_ON;
			wait(0.5);
			ledBus = RED_LED_ON;
			wait(0.5);
    }
		
}
*/