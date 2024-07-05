#include "mbed.h"
#include "BMP180.h"

Serial pc(USBTX, USBRX, 115200);
I2C i2cMaster(I2C_SDA, I2C_SCL); // DATA, CLOCK;
BMP180 bmp180(&i2cMaster);

void setup() {
	pc.printf("BMP180 barometer test >>>\n");
	
	if (bmp180.init() != 0 ){ // ERROR
		pc.printf("Fail to communication with BMP180...\n");
		while (1) {};
	}
	else {
		pc.printf("BMP180 Device initialized...\n");
		pc.printf("BMP180 Device ID : 0x%.2x \n", bmp180.checkId());
		wait(1.0);
	}
}

int main() {
	int pressure = 0;
	float temperature;
	float altitude;
	
	setup();
	
  while(1) {
		bmp180.startTemperature();
		wait_ms(5);
		if (bmp180.getTemperature(&temperature) < 0) {
			pc.printf("Fail to get temperature data...\n");
		}
		
		bmp180.startPressure(BMP180::STANDARD);
		wait_ms(10);
		if (bmp180.getPressure(&pressure) < 0) {
			pc.printf("Fail to get pressure data...\n");
		}
		
		pc.printf("Pressure = %d hPa, Tmeperature = %.2f C\n", pressure, temperature);
		
		altitude = 44330.0f * (1.0f - pow((pressure/102710.0f), (1.0f/5.255f)));
		pc.printf("Altitude = %.1fm\r\n", altitude);

		wait(2.0);
  }
}