#include "mbed.h"

#define PIR_SENSOR_PIN PB_4
#define GREEN_LED PA_13

Serial pc(USBTX, USBRX, 115200);
//DigitalIn motion(PIR_SENSOR_PIN);
InterruptIn motion(PIR_SENSOR_PIN);
DigitalOut light(GREEN_LED);

Timer lightTm;
bool lightOn = false;
int cnt = 0;

void irqRiseHandler(){
	light =1;
	lightOn = true;
	lightTm.reset();
	pc.printf("Detect %d-th motion\n", ++cnt); 
}

void irqFallHandler(){
	light = 0;
	lightOn = false;
}

void setup(){
	lightTm.start();
	motion.rise(irqRiseHandler);
	motion.fall(irqFallHandler);
}

int main() {
		setup();
	
    while(1) {
			/*
			int value = motion;
			if(value == 1 && lightOn == false){
				light = 1;
				lightOn = true;
				lightTm.reset();
				pc.printf("Detect %d-th motion\n", ++cnt); 
			}
			
			else if (lightOn == true) {
				if (lightTm.read() > 3){
					light = 0;
					lightOn = false;
				}
			}
			*/
			
			wait_ms(1000);
		}
	
}
