#include "mbed.h"
#include "open_state.h"
#include "motordriver.h"
#include "buzzer_control.h"

extern Motor motor;
extern enum { OPEN_STATE, LOCK_STATE } DOORLOCK_STATE;

static Timeout stopTimeout;
static Timeout autoTimeout;

void initCloseState();

void motorStop(){
	motor.stop();	
	if(DOORLOCK_STATE == OPEN_STATE) {
			autoTimeout.attach(&initCloseState, 30);
	}		
}

void initOpenState(){
	DOORLOCK_STATE = OPEN_STATE;
	motor.forward(0.5);
	stopTimeout.attach(&motorStop, 5);
	playOpenMelody();
};

void initCloseState() {
	DOORLOCK_STATE = LOCK_STATE;
	motor.backward(0.5);
	stopTimeout.attach(&motorStop, 5);
	playCloseMelody();
}

void cleanupOpenState() {
	autoTimeout.detach();
};

void doOpenStateOperation();
