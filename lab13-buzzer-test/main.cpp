#include "mbed.h"

#define BUZZER_PIN PC_9
#define RED_LED PA_4

PwmOut buzzer(BUZZER_PIN);
DigitalOut redLed(RED_LED);

#define NOTE_C_PERIOD 3830 // msec

Timer buzzerTimer;
bool buzOn = false;

void displayBeep(){
    for(int i=0; i<=2000; i++) { // about 6 second
			buzzer = 1;
			wait_us(1915);
			buzzer = 0;
			wait_us(1915);
    }	
}

void playBeep(bool on) {
	if (on) {
		buzzer = 0.5;
	}
	else {
		buzzer = 0;
	}
}

void toggleRedLed() {
		redLed = !redLed;
		wait(1.0);
}

void setup() {
	buzzer.period_us(NOTE_C_PERIOD);
	buzzer.write(0);
	buzzerTimer.start();
	redLed = 1;
}

int main() {
		setup();
		while(1) {
			toggleRedLed();
			// displayBeep();
			if(buzzerTimer.read() >= 1.0){
				buzOn = !buzOn;
				playBeep(buzOn);
				buzzerTimer.reset();
			}
		}
}
