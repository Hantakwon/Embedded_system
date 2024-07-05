#include "mbed.h"
#include "Adafruit_SSD1306.h"

DigitalOut myled(LED1);

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
Adafruit_SSD1306_I2c Oled(gI2C, D13, 0x78, 64, 128);

int main()
{   uint16_t x=0;

    Oled.printf("%ux%u OLED Display\r\n", Oled.width(), Oled.height());
    Oled.setTextSize(1);
    while(1)
    {
        myled = !myled;
        Oled.printf("hello %u\r",x);
        Oled.display(); // need!
        x++;
        wait(1.0);
    }
}