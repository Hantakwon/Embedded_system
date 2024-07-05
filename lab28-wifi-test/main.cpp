#include "mbed.h"

//#define BYPASS_MODE

Serial pc(USBTX, USBRX, 115200);
Serial wifi(PA_9, PA_10, 9600);

Timer t;

char ssid[] = "SILLA18";
char passwd[] = "silla1234";

void bypass() {
	while (1) {
		while (pc.readable()) {
			wifi.putc(pc.getc());
		}
		
		while (wifi.readable()) {
			pc.putc(wifi.getc());
		}
	}
}

void sendCommand(char *cmd){
	if (cmd != '\0') {
		while (*cmd != '\0') {
			wifi.putc(*cmd++);
		}
	}
}

char buffer[2048];

void outputReply(int timeoutMS) {
	bool ok = false;
	char ch;
	char *cp = buffer;
	
	t.reset();

	while (t.read_ms() < timeoutMS && !ok){
		if (wifi.readable()) {
			while (wifi.readable()) {
				ch = wifi.getc();
				*cp++ = ch;
				if (ch == '\n') {
					*cp = '\0';
					if (strncmp(buffer, "OK", 2) == 0){
						ok = true;
					}
					pc.printf("%s", buffer);
					cp = buffer;
				}
			}
		}
	}
	if (!ok) {
		pc.printf("Timeout in receiving reply\n");
	}
}

void configWiFi(){
	sendCommand("AT\r\n");
	outputReply(1000);
	
	sendCommand("AT\r\n");
	outputReply(1000);
	
	sendCommand("AT\r\n");
	outputReply(1000);
	
	// reset WiFi Module
	// sendCommand("AT+RST\r\n");
	// outputReply(5000);

	// get version info
	sendCommand("AT+GMR\r\n");
	outputReply(1000);
	
	// set operation mode
	// AT+CWMODE=1 (STA), 2(AP), 3(DUAL)
	sendCommand("AT+CWMODE=1\r\n");
	outputReply(1000);
	
	// set connection mode
	sendCommand("AT+CIPMUX=1\r\n");
	outputReply(1000);
	
	
	// get AP list
	sendCommand("AT+CWLAP\r\n");
	outputReply(10000);
		
	// join to AP
	// AT+CWJAP="ssid","password"
	sprintf(buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, passwd);
	sendCommand(buffer);
	outputReply(10000);
	
	// get IP information
	sendCommand("AT+CIFSR\r\n");
	outputReply(1000);
	
	// get connection status
	sendCommand("AT+CIPSTATUS\r\n");
	outputReply(1000);	
}

void setup() {
	t.start();
}
int main() {
	
	setup();
	
	while(1) {
		#ifdef BYPASS_MODE
			bypass();
		#else
			configWiFi();
			wait(5.0);
		#endif
	}		
}

