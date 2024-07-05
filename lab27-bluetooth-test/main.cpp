#include "mbed.h"
#include <math.h>
#include "ctype.h"
#include "motordriver.h"

#define BT_TX 			PA_11
#define BT_RX 			PA_12
#define GREEN_LED		PA_13
#define YELLOW_LED	PB_10
#define RED_LED			PA_4

Serial pc(USBTX, USBRX, 115200);
Serial bt(BT_TX, BT_RX, 9600); // 38400

DigitalOut greenLed(GREEN_LED);
DigitalOut yellowLed(YELLOW_LED);
DigitalOut redLed(RED_LED);

Motor motor(MOTOR_PWM, MOTOR_DIR);

void bypass() {
	// bypass mode
	while (pc.readable()){
		bt.putc(pc.getc());
	}
	while (bt.readable()){
		pc.putc(bt.getc());
	}
}

void controlLed(int led, bool onoff){
	switch(led) {
		case 1:
			greenLed = (onoff ? 1 : 0);
			bt.printf("Green LED : %s\n", (onoff ? "on" : "off"));
			break;
		case 2:
			yellowLed = (onoff ? 1 : 0);
			bt.printf("Yellow LED : %s\n", (onoff ? "on" : "off"));
			break;
		case 3:
			redLed = (onoff ? 1 : 0);
			bt.printf("RED LED : %s\n", (onoff ? "on" : "off"));
			break;
		default :
			break;
	}
}

void controlMotor(int value){
	if (value == 0)
		motor.stop();
	else{
		double speed = double(abs(value) / 100.0);
		if (value > 0)
			motor.forward(speed);
		else if (value<0)
			motor.backward(speed);
	}	
}
void executeCommand(char * cmd) {
	int led = 0;
	bool onoff = false;
	
	char * tok = strtok(cmd, "=");
	if (tok != NULL && strncmp(tok, "LED", 3) == 0){
		led = tok[3] - 0x30;
		tok = strtok(NULL, " ");
		if (strcmp(tok, "ON") == 0) onoff = true;
		else if (strcmp(tok, "OFF") == 0) onoff = false;
		else led = 0;
	}
	else if (tok != NULL && strncmp(tok, "MOT", 3) == 0){	
		tok = strtok(NULL, " ");
		int speed;
		if (tok != NULL) {
			speed = atoi(tok);
		}
		controlMotor(speed);
		return;
	}
	if (led != 0){
		controlLed(led, onoff);
	}
	else {
		bt.printf("Incorrect command!\r\n");
	}
}

char cmdbuf[100];

void getCommandLine(){
	static char *cp = cmdbuf;
	char ch;
	
	while (bt.readable()){
		ch = bt.getc();
		if (ch == ' ' || ch == '\t' || ch == '\r')
			continue;
		
		if (ch == '\n') {
			*cp = '\0';
			executeCommand(cmdbuf);
			cp = cmdbuf;
		}
		else {
			*cp++ = toupper(ch);
		}
	}
}

int main() {
	
	while(1) {
		#ifdef BYPASS_MODE
			bypass();
		#else
			getCommandLine();
		#endif
	}		
}
