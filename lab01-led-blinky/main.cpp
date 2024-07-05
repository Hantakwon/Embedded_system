#include "mbed.h"

DigitalOut led(PA_5);

int main() {
	
    while(1) {
		//led = 1; // led.write(1);
		//wait(0.05); // cpu time break; not good 
		//led = 0;
		//wait(0.05);
		led = !led; // led.write = (!led.lead()):
		wait(0.2);
    }
}
