#include "mbed.h"
#include "password_mode.h"
#include "Adafruit_SSD1306.h"

extern Adafruit_SSD1306_I2c oled;

static int pw[4] = { 0, 0, 0, 0 };
static int ppw[4] = { -1, -1, -1 , -1 };

static const int xp[4] = { 1, 32, 64, 96};
static const int yp = 21;

static int prevCursorPos = -1;
static int currCursorPos = 0;
static int cursor_yp = 61;

#define CURSOR_WIDTH  26
#define CURSOR_HEIGHT 2

void displayPassword() {
	bool pwRefreshed = false;
	
	for (int i=0; i<4; i++) {
		if (pw[i] != ppw[i]) {
			oled.setTextCursor(xp[i], yp);
			oled.writeChar(0x30 + pw[i]);
			ppw[i] = pw[i];
			pwRefreshed = true;
		}
	}
	if (pwRefreshed) {
		oled.display();
	}
}

void displayCursor() {
	if (currCursorPos != prevCursorPos) {
		if (prevCursorPos >= 0) {
			oled.fillRect(xp[prevCursorPos]+1, cursor_yp, CURSOR_WIDTH, CURSOR_HEIGHT, 0);
			oled.display();
		}
		
		oled.fillRect(xp[currCursorPos]+1, cursor_yp, CURSOR_WIDTH, CURSOR_HEIGHT, 1);
		oled.display();
		
		prevCursorPos = currCursorPos;
	}
};


typedef enum { NEUTRAL = 0, LEFT = 1, RIGHT = -1, UP = 1, DOWN = -1 } joystick_value; 
#define NEUTRAL_VALUE 73

static AnalogIn xAsix(PC_2);
static AnalogIn yAsix(PC_3);
static Ticker joystickTicker;
static Ticker updateTicker;
static volatile joystick_value x, y;

void joystickTickerHandler(){
	int x_value = NEUTRAL_VALUE - int(xAsix.read() * 100);
	int y_value = NEUTRAL_VALUE-2 - int(yAsix.read() * 100);

	if (abs(x_value) <= 2) x = NEUTRAL;
	else if (x_value > 0) x = LEFT;
	else x = RIGHT;
	
	if (abs(y_value) <= 2) y = NEUTRAL;
	else if (y_value > 0) y = UP;
	else y = DOWN;
};

void updatePasswordAndCursor(){
	if (y != NEUTRAL) {
		int num = pw[currCursorPos];
		if (y == UP) {
			if (++num > 9) num = 0;
		}
		else if (y == DOWN) {
			if (--num < 0) num = 0;
		}
		pw[currCursorPos] = num;
	}
	else if (x != NEUTRAL) {
		if (x == LEFT) {
			if (--currCursorPos < 0) currCursorPos = 3;
		}
		else if (x == RIGHT) {
			if (++currCursorPos > 3) currCursorPos = 0;
		}
	}
}

int decidePassword() {
	int password = 0;
	for(int i=0; i<4; i++) {
		password = password + pw[3-i] * pow(10.0, i);
	}
	return password;
}

void initPasswordMode(){
	oled.clearDisplay();
		
	oled.setTextSize(1);
	oled.setTextCursor(0,0);
	oled.printf("[ Password Mode ]\n");
	oled.display();
	
	oled.setTextSize(5);
	
	for (int i=0; i<4; i++) {
		ppw[i] = -1;
		pw[i] = 0;
	}
	
	prevCursorPos = -1;
	currCursorPos = 0;
	
	joystickTicker.attach(&joystickTickerHandler, 0.2);
	updateTicker.attach(&updatePasswordAndCursor, 0.4);
};

void cleanupPasswordMode(){
	joystickTicker.detach();
	updateTicker.detach();
};

void doPasswordModeOperation(){
	displayPassword();
	displayCursor();
};
