#include "mbed.h"

Serial pc(USBTX, USBRX);

int counter = 0;

void setup() {
	pc.baud(115200);
	
	pc.printf("Start...\n");
}
int main() {
	setup();
	while(1) {
		
		/*
		while (pc.readable() > 0) {
			pc.putc(pc.getc());
		}
		*/
		pc.printf("%d\r", counter);
		wait(1.0);
		++counter;
	}
}
