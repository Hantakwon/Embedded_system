#include "mbed.h"
#include "pw_mode.h"
#include "Adafruit_SSD1306.h"

extern Adafruit_SSD1306_I2c oled;

void initPWMode(){
	oled.clearDisplay();
		
	oled.setTextSize(1);
	oled.setTextCursor(0,0);
	oled.printf("[ Mode 2 ]\n");
	oled.display();
	
	oled.setTextSize(5);
};

void cleanupPWMode();
void doPWModeOperation();
