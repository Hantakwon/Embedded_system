#ifndef _MOTORDRIVER_H_
#define _MOTORDRIVER_H_

// #progma once

#include "mbed.h"

#define FORWARD_DIR 0
#define BACKWARD_DIR 1

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