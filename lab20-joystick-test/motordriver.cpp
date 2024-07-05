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
void Motor::incSpeed(double step){
	_speed += step;
	if (_speed > 1.0) _speed = 1.0; 
	_pwn = _speed;
}
void Motor::decSpeed(double step) {
	_speed -= step;
	if (_speed < 0.0) _speed = 0.0; 
	_pwn = _speed;
}
void Motor::setDir(int dir) {
	if (_speed == 0) {
		_sign = dir;
		_dir = _sign;
	}
	else if (_sign != dir) {
		_pwn = 0.0;
		wait(0.1);
		
		_sign = dir;
		_dir = _sign;
		_pwn = _speed;
	}
}