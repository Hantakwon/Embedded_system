#include "mbed.h"
#include "button_input.h"

DigitalIn firstBtn(FIRST_BTN);
DigitalIn secondBtn(SECOND_BTN);
DigitalIn thirdBtn(THIRD_BTN);

bool firstButtonPressed(){
	static int prev_value = 1;
	int curr_value = firstBtn.read();
	if(curr_value != prev_value) {
		wait_ms(25);
		prev_value = curr_value;
		if(curr_value == 0) {
			return true;
		}
	}	
	return false;
};

bool secondButtonPressed(){	
	static int prev_value = 1;
	int curr_value = secondBtn.read();
	if(curr_value != prev_value) {
		wait_ms(25);
		prev_value = curr_value;
		if(curr_value == 0) {
			return true;
		}
	}	
	return false;
};

bool thirdButtonPressed(){
	static int prev_value = 1;
	int curr_value = thirdBtn.read();
	if(curr_value != prev_value) {
		wait_ms(25);
		prev_value = curr_value;
		if(curr_value == 0) {
			return true;
		}
	}	
	return false;
};