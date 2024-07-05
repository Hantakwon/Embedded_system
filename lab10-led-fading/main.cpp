#include "mbed.h"

#define YELLOW_LED PB_10

PwmOut myled(YELLOW_LED);

int main() {
	//int period = 1000; //usec
	//int width = 10;
	myled.period_us(1000);
	
	while(1) {
		/*
		myled = 1;
		wait_us(width);
		myled = 0;
		wait_us(period-width);
		*/
		for (float d=0.0; d<=1.0; d+=0.01){
			myled.write(d);
			wait(0.5);
		}
		wait(1.0);
	}
}
