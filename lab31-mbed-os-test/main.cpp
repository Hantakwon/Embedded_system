#include "mbed.h"

#define RTOS_THREAD_TEST

DigitalOut greenLed(PA_13);


#ifdef RTOS_THREAD_TEST

Thread t;

void toggleGreenLed() {
	while (1) {
		greenLed = !greenLed;
		ThisThread::sleep_for(500);
	}
}

int main()
{
#ifdef MBED_MAJOR_VERSION
  printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
  
	t.start(toggleGreenLed);
 	
	while (1) {
		printf("Greeen LED - %s (thread iD: %p)\n", 
			greenLed ? "ON" : "OFF", ThisThread::get_id());		
		ThisThread::sleep_for(300);
	}
}

#else

int main()
{
#ifdef MBED_MAJOR_VERSION
  printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
	
	while (1) {
		greenLed = !greenLed;
		ThisThread::sleep_for(500);
		printf("Greeen LED - %s (thread iD: %p)\n", 
			greenLed ? "ON" : "OFF", ThisThread::get_id());		
	}
}

#endif
