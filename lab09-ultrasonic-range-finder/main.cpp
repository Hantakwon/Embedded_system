#include "mbed.h"

#define URF_TRIG PB_6
#define URF_ECHO PA_8

#define GREEN_LED_FIN PA_13
#define YELLOW_LED_FIN PB_10
#define RED_LED_FIN PA_4

BusOut ledBus(RED_LED_FIN, YELLOW_LED_FIN,GREEN_LED_FIN);

Serial pc(USBTX, USBRX);
DigitalOut urfTrig(URF_TRIG);
DigitalIn urfEcho(URF_ECHO);


Timer tmr;


int echoPulseLength = 0; // usec;
float dist = 0.0; // cm;

void setup() {
	pc.baud(115200);
	pc.printf("*** Ultrasonic ranger finder test ***\n");

	urfTrig = 0;
	
	tmr.start();
}

int main() {
	
	setup();
	
	while(1) {
		// output trig purse signal
		urfTrig = 1;
		wait_us(40);
		urfTrig = 0;
		
		// wait for echo signal to be high
		while (!urfEcho);
		
		// start time measurement
		tmr.reset();
		while (urfEcho);
		
		echoPulseLength = tmr.read_us();
		pc.printf("Echo pulse lenght : %d usec\n", echoPulseLength);
		
		if (echoPulseLength > 18000) {
			pc.printf("Eligeal echo signal input...\n\n");
			ledBus = 0;
		}
		
		else {
			dist = (float)echoPulseLength / (float)58;
			pc.printf("Distance : %5.2f\n\n", dist);
			if ( dist < 20) {
				ledBus = 7;
			}
			else if ( dist > 20 && dist < 50) {
				ledBus = 3;
			}
			else {
				ledBus = 1;
			}
		}
		
		wait(0.5);
	}	
}
