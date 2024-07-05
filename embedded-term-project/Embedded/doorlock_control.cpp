#include "mbed.h"
#include "doorlock_control.h"
#include "motordriver.h"
#include "buzzer_control.h"

Motor motor(MOTOR_PWM, MOTOR_DIR);

extern enum { OPEN_STATE, LOCK_STATE } DOORLOCK_STATE;
extern bool melodyPlay;

static Timeout stopTimeout;
static Timeout autoTimeout;

void autoLockState();

void motorStop(){
	motor.stop();	
	if(DOORLOCK_STATE == OPEN_STATE) {
			autoTimeout.attach(&autoLockState, 30);
	}		
}

void initOpenState(){
	motor.forward(0.5);
	stopTimeout.attach(&motorStop, 5);
	DOORLOCK_STATE = OPEN_STATE;
};

void initLockState() {
	motor.backward(0.5);
	stopTimeout.attach(&motorStop, 5);
	DOORLOCK_STATE = LOCK_STATE;
}

void autoLockState() {
	motor.backward(0.5);
	stopTimeout.attach(&motorStop, 5);
	DOORLOCK_STATE = LOCK_STATE;
	melodyPlay = false;
}

void cleanupOpenState() {
	autoTimeout.detach();
};