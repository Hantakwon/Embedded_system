#include "mbed.h"
#include "accelerometer.h"

Serial pc(USBTX, USBRX, 115200);

float accelData[3];

void setup() {
	initSPIMaster();
	initAcceleroMeter();
	
	pc.printf("Start...\n");
}
int main() {
	setup();
	
	pc.printf("Accelerometer Device ID : 0x%2x \n", getAccelDevId()); // two 116
  while(1) {
		getAccelData(accelData);
		pc.printf("X = %+.2fg, Y = %+.2fg, Z = %+.2fg \n",
			accelData[0], accelData[1], accelData[2]);
		wait(0.2);
	}
}
