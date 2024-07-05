#include "motordriver.h"
	
Motor::Motor(PinName pwn, PinName dir)
: _pwn(pwn), _dir(dir)
{
	_sign = FORWARD_DIR; // clock
	_dir.write(FORWARD_DIR);
 
	_speed = 0.0;
	_pwn.period_ms(1); // 1000Hz
	_pwn.write(_speed);
}

Motor::~Motor() {}

void Motor::forward(double speed){ // clock
	if (_sign == FORWARD_DIR && _speed == speed)
		return;
	if(_sign == BACKWARD_DIR){
		_pwn = 0.0;
		wait_ms(50);
	}
	
	_dir = FORWARD_DIR;
	_pwn = speed;
	_sign = FORWARD_DIR;
	_speed = speed;
	
}
void Motor::backward(double speed){
	if (_sign == BACKWARD_DIR && _speed == speed)
		return;
	if(_sign == FORWARD_DIR){
		_pwn = 0.0;
		wait_ms(50);
	}
	_dir = BACKWARD_DIR;
	_pwn = speed;
	_sign = BACKWARD_DIR;
	_speed = speed;
}

void Motor::stop(){
	_speed = 0.0;
	_pwn = _speed;
}