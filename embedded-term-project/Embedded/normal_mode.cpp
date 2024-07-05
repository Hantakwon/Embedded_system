#include "mbed.h"
#include "normal_mode.h"
#include "DHT.h"
#include "Adafruit_SSD1306.h"

extern Adafruit_SSD1306_I2c oled;
extern enum { OPEN_STATE, LOCK_STATE } DOORLOCK_STATE;
extern Serial bt;

DHT dht22(PB_2, DHT22);

static Ticker DHTTicker;
float temp, humidity;

void DHTTickerHandler() {
	if(dht22.readData() == ERROR_NONE){
			temp = dht22.ReadTemperature(CELCIUS);
			humidity = dht22.ReadHumidity();
	}
}

void initNormalMode(){
	oled.clearDisplay();
		
	oled.setTextSize(1);
	oled.setTextCursor(0,0);
	oled.printf("[ Normal Mode ]\n");
	oled.display();
	
	DHTTicker.attach(&DHTTickerHandler, 1.0);
};

void cleanupNormalMode(){
	DHTTicker.detach();
};

void doNormalModeOperation(){
	static float prev_temp = 0;
	static float prev_humidity = 0;
	static int prev_doorlock_state = LOCK_STATE;
	
	if (temp != prev_temp || humidity != prev_humidity || DOORLOCK_STATE != prev_doorlock_state) {
		prev_temp = temp;
		prev_humidity = humidity;
		prev_doorlock_state = DOORLOCK_STATE;
		oled.setTextCursor(0,10);
		switch(DOORLOCK_STATE){
			case OPEN_STATE :
				oled.printf("DOORLOCK : OPEN\r");
				bt.printf("door=OPEN\n");
				break;
			case LOCK_STATE :
				oled.printf("DOORLOCK : LOCK\r");
				bt.printf("door=LOCK\n");
				break;
		}
		oled.setTextCursor(0,20);
		oled.printf("temp : %.1fC\r", temp);
		bt.printf("temp=%.1f \n", temp);
		oled.setTextCursor(0,30);
		oled.printf("humidity : %.1f%%\r", humidity);
		bt.printf("humidity=%.1f \n", humidity);
		oled.display();
	}
};