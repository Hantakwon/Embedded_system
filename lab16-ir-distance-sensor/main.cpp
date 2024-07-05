#include "mbed.h"

#define IR_SENSOR_PIN PC_5

Serial pc(USBTX, USBRX);
AnalogIn IrSensor(IR_SENSOR_PIN);

int main() {
  pc.baud(115200);
  pc.printf("IR Distance Sensor Test!!\r\n");
  while(1) {
		
		double value = 0; 
		
		for (int i=0; i<20; i++) { 
			value += IrSensor.read();
			wait_ms(5);
		}
		value /= 20;
		// average Filter to remove noise
		
		if (value >= 0.121 && value <= 0.97) {
			double volt = value * 3.3;
			//dist = 27.61f / (volt - 0.1696f);
			double dist = 26.663 * pow(volt, -1.25);
			pc.printf("volt : %5.3f distance : %5.2f cm\n",volt, dist);
		}

		else pc.printf("No object are detected!....\n");
		

		wait_ms(100);
  }
}
