#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include "mbed.h"

#define SPI_CLK PB_13 // Clock
#define SPI_MISO PB_14 // Master Input
#define SPI_MOSI PB_15 // Master Output
#define ACCEL_CS PB_12

SPI spiMaster(SPI_MOSI, SPI_MISO, SPI_CLK); // Master
DigitalOut csAccel(ACCEL_CS); // Low Active

#define SELECT_ACCEL_DEV() csAccel = 0
#define DESELECT_ACCEL_DEV() csAccel = 1

void initSPIMaster(){
	// disable all slave devices
	DESELECT_ACCEL_DEV();
	
	// setup SPI master parameters
	spiMaster.format(8, 3);
	spiMaster.frequency(1000000);
}

void initAcceleroMeter(){
	SELECT_ACCEL_DEV();
	
	spiMaster.write(0x00 | 0x00 | 0x31); // send register address
	spiMaster.write(0x01); // send data
	
	spiMaster.write(0x00 | 0x00 | 0x2D); // 0 = write 8 = read | single or multi | register address
	spiMaster.write(0x08); // send data
	
	DESELECT_ACCEL_DEV();
}

int getAccelDevId() {
	SELECT_ACCEL_DEV();

	spiMaster.write(0x80 | 0x00 | 0x00); // read 0 register adderss
	int id =  spiMaster.write(0x00); // send dummy data
	
	DESELECT_ACCEL_DEV();
	
	return id;
}

void getAccelData(float *accelData) {
	char buffer[6];
	int16_t data;
	
	SELECT_ACCEL_DEV();

	spiMaster.write(0x80 | 0x40 | 0x32); // read 0 register adderss
	for(int i=0; i<6; i++){
		buffer[i] = spiMaster.write(0x00);
	}
	DESELECT_ACCEL_DEV();
	
	data = (buffer[1] << 8) | buffer[0];
	accelData[0] = data * 0.0078; // databook gravity accelerometer
	data = (buffer[3] << 8) | buffer[2];
	accelData[1] = data * 0.0078;
	data = (buffer[5] << 8) | buffer[4];
	accelData[2] = data * 0.0078; 
}

#endif