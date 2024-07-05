#include "mbed.h"
#include "motordriver.h"

Motor motor(PA_7, PC_8);

int main() {

    while(1) {
				for (float s=0.0; s<=1.0; s+=0.05){
					motor.forward(s);
					wait(0.5);
				}
				for (float s=1.0; s>=0.0; s-=0.05){
					motor.forward(s);
					wait(0.5);
				}
				wait(2.0);
				for (float s=0.0; s<=1.0; s+=0.05){
					motor.backward(s);
					wait(0.5);
				}
				for (float s=1.0; s>=0.0; s-=0.05){
					motor.backward(s);
					wait(0.5);
				}
				wait(2.0);
    }		
}
