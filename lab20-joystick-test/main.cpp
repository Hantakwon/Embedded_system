#include "mbed.h"
#include "motordriver.h"

#define JOYSTICK_X PC_2
#define JOYSTICK_Y PC_3
#define MOTOR_A_PWM PA_7
#define MOTOR_A_DIR PC_8
#define FIRST_BTN PA_14

Serial pc(USBTX, USBRX, 115200);
AnalogIn joyX (JOYSTICK_X);
AnalogIn joyY (JOYSTICK_Y);
InterruptIn stopBtn(PA_14); 

Motor motorA(MOTOR_A_PWM, MOTOR_A_DIR);

Ticker joyTicker;
Ticker motorTicker;

#define JOYSTICK_CENTER_VALUE_X 72
#define JOYSTICK_CENTER_VALUE_Y 69
int x, y;

void readJoystickInput() {
	x = joyX.read() * 100;
	y = joyY.read() * 100;
	
	x -= JOYSTICK_CENTER_VALUE_X;
	if (abs(x) <= 2) x = 0;
	y -= JOYSTICK_CENTER_VALUE_Y;
	if (abs(y) <= 2) y = 0;
}

void stopMotor() {
	motorA.stop();
}

void controlMotorA() {
	pc.printf("X: %d, Y: %d\n", x, y);
	
	if (x > 0) motorA.setDir(FORWARD_DIR);
	else if (x < 0) motorA.setDir(BACKWARD_DIR);
	
	if (y > 0) motorA.decSpeed(0.05);
	else if (y < 0) motorA.incSpeed(0.05);
}

void setup(){
	joyTicker.attach(&readJoystickInput, 0.05);
	motorTicker.attach(&controlMotorA, 0.1);
	stopBtn.fall(&stopMotor);
}

int main() {
  setup();  
	while(1) {
		/*
			readJoystickInput();
			pc.printf("X: %d, Y: %d\n", x, y);
			pc.printf("Y : %s, X : %s\n", 
				(x == 0 ? "CENTER" : (x < 0 ? "LEFT" : "RIGHT")),
				(y == 0 ? "CENTER" : (y < 0 ? "UP" : "DOWN")));
		*/	
		wait(5);
	}		
}
