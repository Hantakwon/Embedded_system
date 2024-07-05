#ifndef _MOTORDRIVER_H_
#define _MOTORDRIVER_H_

#include "mbed.h"

class Motor {
	public:
		Motor(PinName pwn, PinName dir);
		~Motor();
		void forward(double speed);
		void backward(double speed);
		void stop();
	
	private:
		PwmOut _pwn;
		DigitalOut _dir;
		int _sign;
		double _speed;
};

#endif // _MOTORDERIVER_H_