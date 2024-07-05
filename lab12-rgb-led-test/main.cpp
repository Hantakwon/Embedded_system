#include "mbed.h"

#define RGB_RED PA_1
#define RGB_GREEN PC_6
#define RGB_BLUE PB_0

PwmOut rgbRed(RGB_RED);
PwmOut rgbGreen(RGB_GREEN);
PwmOut rgbBlue(RGB_BLUE);

void setup(){
	rgbRed.period_ms (1);
	rgbGreen.period_ms (1);
	rgbBlue.period_ms (1);
}

int main() {	
    while(1) {
			for (float f=0.0; f<=1.0; f+=0.01) {
				f*=3;
				
				rgbRed = 1.0 - ((f < 1.0) ? f : ((f < 2.0) ? 1.0 : 3.0 - f));
				rgbGreen = (f < 1.0) ? f : ((f < 2.0) ? 1.0 : f - 2.0);
				rgbBlue = (f < 1.0) ? 0 : ((f < 2.0) ? f - 1.0 : f - 2.0);
				
				wait(0.5);
			}
		}
}
