
 /* SENSOR_HMC5883L.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "SENSOR_HMC5883L.h"
#include "HMC5883LPinMap.h"

static void Hmc5883lTimertimeout(void);
static void Hmc5883lStoptimeout(void);
static TIMER_ID Hmc5883lRuntimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime);
static void Hmc5883lISR(void);

static TIMER_ID Hmc5883l_TimerID = INVALID_TIMER_ID;

int headingAngle;
float headingOffset;
Kalman headingkalman;

static void I2CInit(void){
	SysCtlPeripheralEnable(SENSOR_PERIPHERAL);
	SysCtlDelay(2);
	SysCtlPeripheralEnable(SENSOR_ROOT_PERIPHERAL);
	SysCtlDelay(2);

	GPIOPinConfigure(SENSOR_GPIO_SCL);
	GPIOPinConfigure(SENSOR_GPIO_SDA);

	GPIOPinTypeI2CSCL(SENSOR_PORT, SENSOR_PIN_SCL);
	GPIOPinTypeI2C(SENSOR_PORT, SENSOR_PIN_SDA);

	I2CMasterInitExpClk(SENSOR_I2C, SysCtlClockGet(), true);
	SysCtlDelay(2);
//	I2CIntRegister(SENSOR_I2C, &Hmc5883lISR);
//	I2CMasterIntEnable(SENSOR_I2C);

	//Register for compass measurement
//	Hmc5883lRuntimeout(&Hmc5883lTimertimeout, 10);
}

static uint8_t I2CRead(uint8_t address, uint8_t registerAddress){
	I2CMasterSlaveAddrSet(SENSOR_I2C, address, false);

	I2CMasterDataPut(SENSOR_I2C, registerAddress);
	I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(SENSOR_I2C));

	I2CMasterSlaveAddrSet(SENSOR_I2C, address, true);

	I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(SENSOR_I2C));
	return I2CMasterDataGet(SENSOR_I2C);
}

static void I2CReadData(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t datalength){
		I2CMasterSlaveAddrSet(SENSOR_I2C, address, false);

		I2CMasterDataPut(SENSOR_I2C, registerAddress);
		I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_SINGLE_SEND);
		while(I2CMasterBusy(SENSOR_I2C));

		I2CMasterSlaveAddrSet(SENSOR_I2C, address, true);

		I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy(SENSOR_I2C));
		data[0] = I2CMasterDataGet(SENSOR_I2C);
		uint8_t position = 1;
		for (position = 1; position < datalength; position++) {
			I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			while(I2CMasterBusy(SENSOR_I2C))
				data[position] = I2CMasterDataGet(SENSOR_I2C);
		}

		I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		while(I2CMasterBusy(SENSOR_I2C));
		data[datalength - 1] = I2CMasterDataGet(SENSOR_I2C);
}

static void I2CWriteData(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t datalength){
		I2CMasterSlaveAddrSet(SENSOR_I2C, address, false);

		I2CMasterDataPut(SENSOR_I2C, registerAddress);
		I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_BURST_SEND_START);
		while(I2CMasterBusy(SENSOR_I2C));
		uint8_t position = 0;
		for (position = 0; position < datalength - 1; position++) {
			I2CMasterDataPut(SENSOR_I2C, data[position]);
			I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_BURST_SEND_CONT);
			while(I2CMasterBusy(SENSOR_I2C));
		}

		I2CMasterDataPut(SENSOR_I2C, data[datalength - 1]);
		I2CMasterControl(SENSOR_I2C, I2C_MASTER_CMD_BURST_SEND_FINISH);
		while(I2CMasterBusy(SENSOR_I2C));
}
static void I2CWrite(uint8_t address, uint8_t registerAddress, uint8_t data){
	I2CWriteData(address, registerAddress, &data, 1);
}

void Hmc5883lInit(void){
	int16_t xAxixvalue, yAxixvalue, zAxixvalue;
	I2CInit();
	I2CWrite(HMC5883L_ADD, HMC5883L_CFG_A, 0x78);
	SysCtlDelay(SysCtlClockGet()/30);
	I2CWrite(HMC5883L_ADD, HMC5883L_CFG_B, 0x20);
	SysCtlDelay(SysCtlClockGet()/30);
	I2CWrite(HMC5883L_ADD, HMC5883L_CFG_A, 0x00);
	SysCtlDelay(SysCtlClockGet()/30);
	KalmanInit(&headingkalman);
	Hmc5883lMeasurement(&xAxixvalue, &yAxixvalue, &zAxixvalue);
	headingOffset = Hmc5883lCalibration(xAxixvalue, yAxixvalue, zAxixvalue);
	headingAngle = Hmc5883lAzimuth();

	SetAngle(&headingkalman, headingAngle);
}

void Hmc5883lMeasurement(int16_t *xAxixvalue, int16_t *yAxixvalue, int16_t *zAxixvalue){
	uint8_t readData[6];
	I2CReadData(HMC5883L_ADD, HMC5883L_DATA_X_MSB, readData, 6);
	*xAxixvalue = (readData[0] << 8) & 0xff00 | readData[1] & 0xff;
	*yAxixvalue = (readData[2] << 8) & 0xff00 | readData[3] & 0xff;
	*zAxixvalue = (readData[4] << 8) & 0xff00 | readData[5] & 0xff;
}

float Hmc5883lCalibration(int xAxixvalue, int yAxixvalue, int zAxixvalue){
	float headingAngle;
	headingAngle = atan2((float)yAxixvalue, (float)xAxixvalue);
	headingAngle = (headingAngle * 180) / 3.14 + 180;
	if ((int) headingAngle < 68) {
		headingAngle *= 1.3235;
	}
	else {
		if ((int) headingAngle < 131) {
			headingAngle = (headingAngle - 68) * 1.4286 + 90;
	}
	 else {
		if ((int) headingAngle < 231) {
			headingAngle = (headingAngle - 131) * 0.9 + 180;
		} else {
			headingAngle = (headingAngle - 231) * 0.69767 + 270;
		}
	  }
	}
	return headingAngle;
}

int Hmc5883lAzimuth(){
	float headingAngle;
	int16_t xAxixvalue, yAxixvalue, zAxixvalue;
	Hmc5883lMeasurement(&xAxixvalue, &yAxixvalue, &zAxixvalue);
	headingAngle = Hmc5883lCalibration(xAxixvalue, yAxixvalue, zAxixvalue) - headingOffset;
	if (headingAngle < 0.00) {
		headingAngle += 360;
	}
	headingAngle = 360 - headingAngle;
	headingAngle *= 100;
	return (int)headingAngle;
}

static void Hmc5883lISR(void){
	I2CMasterIntClear(SENSOR_I2C);
	Hmc5883lRuntimeout(&Hmc5883lTimertimeout, 10);
	headingAngle = Hmc5883lAzimuth();

	GetAngle(&headingkalman, headingAngle);
}
static void Hmc5883lTimertimeout(void){
	Hmc5883l_TimerID = INVALID_TIMER_ID;
}

static void Hmc5883lStoptimeout(void){
	if (Hmc5883l_TimerID != INVALID_TIMER_ID) {
		TIMER_UnregisterEvent(Hmc5883l_TimerID);
	}
	Hmc5883l_TimerID = INVALID_TIMER_ID;
}
static TIMER_ID Hmc5883lRuntimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime){
	Hmc5883lStoptimeout();
	Hmc5883l_TimerID = TIMER_RegisterEvent(TimeoutCallback, msTime);
	return Hmc5883l_TimerID;
}
