#include "mbed.h"
#include "count_mode.h"
#include "pw_mode.h"
#include "Adafruit_SSD1306.h"

enum { COUNT_MODE, PW_MODE } mode;

// an I2C sub-class that provides a constructed default
class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
        //start();
    };
};

I2CPreInit gI2C(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c oled(gI2C, D13, 0x78, 64, 128);
DigitalIn modeSW(PA_14);

bool isModeSWPressed(){
	static int prev_value = 1;
	int curr_value = modeSW.read();
	if ( curr_value != prev_value) {
		wait_ms(25);
		prev_value = curr_value;
		if ( curr_value == 0 ){
			return true;
		}
	}
	return false;
}
void setup(){
	mode = COUNT_MODE;
	initCountMode();
}

int main()
{   
	setup();
	
  while(1)
  {	
		
		switch(mode){
			case COUNT_MODE : 
				doCountModeOperation();
				if(isModeSWPressed() == true) {
					cleanupCountMode();
					mode = PW_MODE;
					initPWMode();
				}
				break;
			case PW_MODE :
				doPWModeOperation();
				if(isModeSWPressed() == true) {
					cleanupPWMode();
					mode = COUNT_MODE;
					initCountMode();
				}
				break;
			default :
				break;
		}
  }
}