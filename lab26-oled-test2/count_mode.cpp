#include "mbed.h"
#include "count_mode.h"
#include "Adafruit_SSD1306.h"

extern Adafruit_SSD1306_I2c oled;

static Ticker cntTicker;
static long cnt = 0;

void cntTickerHandler() {
	++cnt;
}

void initCountMode(){
	oled.clearDisplay();
		
	oled.setTextSize(1);
	oled.setTextCursor(0,0);
	oled.printf("[ Mode 1 ]\n");
	oled.display();
	
	oled.setTextSize(5);
	
	cntTicker.attach(&cntTickerHandler, 1.0);
};

void cleanupCountMode(){
	cntTicker.detach();
};

void doCountModeOperation(){
	static long prevCnt = -1;
	
	if (cnt != prevCnt) {
		oled.printf("count : %d\r", cnt);
		oled.display();
	}
};