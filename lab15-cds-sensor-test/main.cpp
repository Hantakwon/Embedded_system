#include "mbed.h"

#define CDS_FIN PA_0
#define YELLOW_LED PB_10
#define THRESHOLD 0.61

Serial pc(USBTX, USBRX, 115200);
AnalogIn cdsSensor(CDS_FIN);
PwmOut yellowLed(YELLOW_LED);

int main() {
    while(1) {
			double value = cdsSensor.read();
			if(value >= THRESHOLD){
				yellowLed = value;
			}
			else yellowLed = 0;
			pc.printf("cds Value %5.4f, Voltage=%5.4f\n", value, value * 3.3);
			wait(0.25);
		}		
}
