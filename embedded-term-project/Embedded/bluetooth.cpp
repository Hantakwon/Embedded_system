#include "mbed.h"
#include "bluetooth.h"
#include "ctype.h"

#define BT_TX 			PA_11
#define BT_RX 			PA_12

Serial pc(USBTX, USBRX, 115200);
Serial bt(BT_TX, BT_RX, 9600); // 38400

Ticker commandTicker;

extern bool openD, lockD, normalM, passwordM, correct, incorrect;
extern int PASSWORD;
static int inputData[4];
int data;

void bypass() {
	// bypass mode
	while (pc.readable()){
		bt.putc(pc.getc());
	}
	while (bt.readable()){
		pc.putc(bt.getc());
	}
}

void executeCommand(char * cmd) {
	pc.printf("%s\n", cmd);
	char * tok = strtok(cmd, "=");
	if (tok != NULL){ 
		if (strncmp(tok, "DOOR", 4) == 0){
			tok = strtok(NULL, " ");
			if (strcmp(tok, "OPEN") == 0) {
				openD = true;
				return;
			}
			else if(strcmp(tok, "LOCK") == 0) {
				lockD = true;
				return;
			}
			else{
			bt.printf("Incorrect command!\r\n"); 
			return;
			}
		}
		if (strncmp(tok, "MOD", 3) == 0){
			tok = strtok(NULL, " ");
			if (strcmp(tok, "NORMAL") == 0) {
				normalM = true;
				return;
			}
			else if(strcmp(tok, "PASSWORD") == 0){
				passwordM = true;
				return;
			}
			else{
			bt.printf("Incorrect command!\r\n"); 
			return;
			}
		}
		if (strncmp(tok, "PW", 2) == 0){
			tok = strtok(NULL, " ");
			if(strlen(tok) == 4 ){
				data = atoi(tok);				
				if(data == PASSWORD){
					openD = true;
					correct = true;
					return;
				}
				else { 
					bt.printf("Incorrect password!\r\n");    
					incorrect = true;
					return;
				}
			}
			else{
				bt.printf("Incorrect password!\r\n"); 
				incorrect = true;
				return;
			}
		}
		else{
			bt.printf("Incorrect command!\r\n"); 
			return;
		}
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

void getCommandTicker(){
	commandTicker.attach(&getCommandLine,0.0001);
}