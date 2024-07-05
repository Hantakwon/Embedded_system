#include "mbed.h"
#include "DHT.h"

Serial pc(USBTX, USBRX, 115200);
DHT dht22(PB_2, DHT22);

int main() {
	while(1) {
		if(dht22.readData() != ERROR_NONE){
			pc.printf("Fail to read data in DHT22...\n");
			continue;
		}
		
		float humidity = dht22.ReadHumidity();
		float temp = dht22.ReadTemperature(CELCIUS);
		
		pc.printf("Current Temp : %.1f C, Humidity : %.1f %%\n", temp ,humidity);
		
		wait(1.0);
	}	
}
