#include "mbed.h"
#include "lock_state.h"
#include "motordriver.h"
#include "buzzer_control.h"

extern Motor motor;

static Timeout doorLockControl;

void motorStop(){
	motor.stop();
}

void initOpenState(){
	motor.backward(0.5);
	doorLockControl.attach(&motorStop, 5.0);
	playCloseMelody();
};

void cleanupLockState();
void doLockStateOperation();