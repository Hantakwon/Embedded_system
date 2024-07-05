#include "mbed.h"
#include "light_control.h"
#include "button_input.h"
#include "buzzer_control.h"
#include "Adafruit_SSD1306.h"
#include "doorlock_control.h"
#include "normal_mode.h"
#include "password_mode.h"
#include "bluetooth.h"
#include "wifi.h"

class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
    };
};

Timeout wifiout;
I2CPreInit myI2C(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c oled(myI2C, D13, 0x78, 64, 128);

enum { OPEN_STATE, LOCK_STATE } DOORLOCK_STATE;
enum { NORMAL_MODE, PASSWORD_MODE } OLED_MODE;
int PASSWORD = 1234;
bool melodyPlay = false;
bool openD = false, lockD = false, normalM = false, passwordM = false, correct = false, incorrect = false;

void setOpen() {
	initOpenState();
	cleanupPasswordMode();
  OLED_MODE = NORMAL_MODE;
  initNormalMode(); 
	melodyPlay = false;
}

void setLock() {
	cleanupOpenState();		
  initLockState();
	melodyPlay = false;
}

void setNormal() { 
	cleanupPasswordMode();
  OLED_MODE = NORMAL_MODE;
  initNormalMode();
}

void setPassword() {
	cleanupNormalMode();
  OLED_MODE = PASSWORD_MODE;
  initPasswordMode();
}

void setup() {
	DOORLOCK_STATE = LOCK_STATE;
	OLED_MODE = NORMAL_MODE;
	initNormalMode();
	getCommandTicker();
	melodyPlay = true;
	wait(2.0);
}

int main() {
	
		setup();
	
    while(1) {
			switch(DOORLOCK_STATE){
				case OPEN_STATE :
					if(melodyPlay == false){
						playOpenMelody();
						melodyPlay =true;
					}
					if(thirdButtonPressed() == true) {
						setLock();
					}
					break;
					
				case LOCK_STATE :
					if(melodyPlay == false){
						playLockMelody();
						melodyPlay =true;
					}
					if(firstButtonPressed() == true) {
            switch(OLED_MODE){
							case NORMAL_MODE :
								setPassword();
								break;
							case PASSWORD_MODE :
								setNormal();
                break;
              default :
                break;
            }
					}
					
					else if(OLED_MODE == PASSWORD_MODE && secondButtonPressed() == true) {
						if(PASSWORD == decidePassword()){
							activeLEDOnCorrect();
							setOpen();
						}
						else { 
							activeLEDOnIncorrect();
							playWarningMelody();
						}
					}
					
					else if(thirdButtonPressed() == true) {
						setOpen();	
					}
					break;
				default :
					break;
			};
			
			switch(OLED_MODE){
				case NORMAL_MODE :
					doNormalModeOperation();
					break;
				case PASSWORD_MODE :
					doPasswordModeOperation();
					break;
				default :
					break;
			};
			
			if(openD == true && DOORLOCK_STATE == LOCK_STATE){
				setOpen();
				openD = false;
			}
			if(lockD == true && DOORLOCK_STATE == OPEN_STATE){
				setLock();
				lockD = false;
			}
			if(normalM == true && OLED_MODE == PASSWORD_MODE ){
				setNormal();
				normalM = false;
			}
			if(passwordM == true && OLED_MODE == NORMAL_MODE){
				setPassword();
				passwordM = false;
			}
			if(correct == true){
				activeLEDOnCorrect();
				correct = false;
			}
			if(incorrect == true){
				activeLEDOnIncorrect();
				playWarningMelody();
				incorrect = false;
			}
	}
}