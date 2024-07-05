#ifndef _BUTTON_INPUT_H_
#define _BUTTON_INPUT_H_

#include "mbed.h"

#define EMG_BUTTON PA_14

DigitalIn emgBtn(EMG_BUTTON);

bool emgButtonPressed() {
	static int prevValue = 1;
	int currValue = emgBtn.read();
	if(prevValue!=currValue){
		wait_ms(50);
		prevValue = currValue;
		if(currValue == 0) {
			return true;
		}
	}
	return false;
}

#endif