/*
 * RC522.c
 *
 *  Created on: Aug 29, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "RC522.h"

uint8_t a[6];
uint8_t b[6];
//static void RC522TimerTimeout(void);
//static void RC522StopTimeout(void);
//static TIMER_ID RC522RunTimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime);

static uint8_t SSIReadWrite(uint32_t writedata);
static void RC522WriteRegister(uint8_t address, uint8_t value);
static uint8_t RC522ReadRegister(uint8_t address);
static void RC522ClearBit(uint8_t address, uint8_t mask);
static void RC522SetBit(uint8_t address, uint8_t mask);
static void RC522Reset();
static void RC522AntennaOn();
static void RC522AntennaOff();
static TIMER_ID rc522TimerID = INVALID_TIMER_ID;

uint32_t trashbin[2];

void SPIInit(){
	SysCtlPeripheralEnable(RC522_FUNCTION);
	SysCtlPeripheralEnable(RC522_ALTERNATE_FUNCTION);

	GPIOPinConfigure(GPIO_PB4_SSI2CLK);
	GPIOPinConfigure(GPIO_PB5_SSI2FSS);
	GPIOPinConfigure(GPIO_PB6_SSI2RX);
	GPIOPinConfigure(GPIO_PB7_SSI2TX);

	GPIOPinTypeSSI(RC522_ROOT, RC522_CS_PIN | RC522_MISO_PIN | RC522_MOSI_PIN | RC522_SCK_PIN);
	SSIConfigSetExpClk(RC522_PORT, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 400000, 8);
	SSIEnable(RC522_PORT);

//	SSIIntEnable()
}

static uint8_t SSIReadWrite(uint32_t writedata){
		SSIDataPut( RC522_PORT, writedata );
		if( !SSIDataGetNonBlocking( RC522_PORT, &writedata ) )
		{
			SSIDataGet( RC522_PORT, &writedata);
		}
		return writedata & 0xFF;
}

extern void SSIWrite(uint8_t writedata){
	//Empty TX FIFO
//	while(SSIDataPutNonBlocking(RC522_PORT, trashbin[0]));
	//Send byte through SPI peripheral
	SSIDataPut(RC522_PORT, writedata);
	//Wait until transfer is done
//	while(SSIDataGetNonBlocking(RC522_PORT, &trashbin[0])){};
	//Return the byte read from SPI bus
//	SSIDataGet(RC522_PORT, &trashbin[1]);
//	return trashbin[1];
}

static void RC522WriteRegister(uint8_t address, uint8_t value){
	uint8_t status;
	RC522_CS_LOW();
	while(SSIBusy(RC522_PORT));
	SSIReadWrite((address << 1) & 0x7E);

	SSIReadWrite(value);
	RC522_CS_HIGH();
}

static uint8_t RC522ReadRegister(uint8_t address){
	 uint8_t value;
	 RC522_CS_LOW();
	 while(SSIBusy(RC522_PORT));
	 value = SSIReadWrite(((address << 1) & 0x7E) | 0x80);
	 while(SSIBusy(RC522_PORT));
	 value = SSIReadWrite(0x00);
	 RC522_CS_HIGH();

	 return value;
}

static void RC522ClearBit(uint8_t address, uint8_t mask){
	RC522WriteRegister(address, RC522ReadRegister(address) & ~(mask));
}

static void RC522SetBit(uint8_t address, uint8_t mask){
	RC522WriteRegister(address, RC522ReadRegister(address) | mask);
}

static void RC522Reset(){
	RC522WriteRegister(COMMANDREG, PCD_RESETPHASE);
}

static void RC522AntennaOn(){
	uint8_t temp;
	temp = RC522ReadRegister(TXCONTROLREG);
	if (!(temp & 0x03)) {
		RC522SetBit(TXCONTROLREG, 0x03);
		temp = RC522ReadRegister(TXCONTROLREG);
	}
}

static void RC522AntennaOff(){
	RC522ClearBit(TXCONTROLREG, 0x03);
}

void RC522Init(){
	SPIInit();
	RC522_CS_HIGH();

	RC522Reset();//Reset

	RC522WriteRegister(TMODEREG, 0x8D); //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	b[0] = RC522ReadRegister(TMODEREG);
	RC522WriteRegister(TPRESCALERREG, 0x3E);
	b[1] = RC522ReadRegister(TPRESCALERREG);
	RC522WriteRegister(TRELOADREGL, 30);
	b[2] = RC522ReadRegister(TRELOADREGL);
	RC522WriteRegister(TRELOADREGH, 0);
	b[3] = RC522ReadRegister(TRELOADREGH);
	RC522WriteRegister(TXAUTOREG, 0x40); //100% ACK
	b[4] = RC522ReadRegister(TXAUTOREG);
	RC522WriteRegister(MODEREG, 0x3D); //CRC valor initial 0x6363
	b[5] = RC522ReadRegister(MODEREG);

	RC522AntennaOff();
	RC522AntennaOn();
}

RC522Status_t RC522ToCard(uint8_t command, uint8_t *DataSend, uint8_t datasendlength, uint8_t *databack, uint16_t *DataBackLength){
	RC522Status_t _status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIrq = 0x00;
	uint8_t lastbits;
	uint8_t n;
	uint16_t count;
	switch (command) {
		case PCD_AUTHENT:
			irqEn = 0x12;
			waitIrq = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn = 0x77;
			waitIrq = 0x30;
			break;
		default:
			break;
	}

	RC522WriteRegister(COMMIENREG, irqEn | 0x80);//Interrupt Request
	RC522ClearBit(COMMIRQREG, 0x80);
	RC522SetBit(FIFOLEVELREG, 0x80);
	RC522WriteRegister(COMMANDREG, PCD_IDLE);

	//Writing data to the FIFO
	for (count = 0; count < datasendlength; count++) {
		RC522WriteRegister(FIFODATAREG, DataSend[count]);
	}
	//Execute the command
	RC522WriteRegister(COMMANDREG, command);
	if (command == PCD_TRANSCEIVE) {
		RC522SetBit(BITFRAMINGREG, 0x80);
		a[0] = RC522ReadRegister(BITFRAMINGREG);
	}
	//Waiting to receive data to complete
	//count according to  the clock frequency adjustment, the operator M1 card maximum waiting time 25ms

	count = 2000;
	do {
		n = RC522ReadRegister(COMMIRQREG);
		count--;
	} while (count && !(n & 0x01) && !(n & waitIrq));

	RC522ClearBit(BITFRAMINGREG, 0x80); //StartSend = 0
	a[1] = RC522ReadRegister(BITFRAMINGREG);
	if (count != 0) {
		if (!(RC522ReadRegister(ERRORREG) & 0x1B)) {
			_status = MI_OK;
			if (n & irqEn & 0x01) {
				_status = MI_NOTAGERR;
			}
			if (command == PCD_TRANSCEIVE) {
				n = RC522ReadRegister(FIFOLEVELREG);
				lastbits = RC522ReadRegister(CONTROLREG) & 0x07;

				if (lastbits) {
					*DataBackLength = (n - 1) * 8 + lastbits;
				}
				else {
					*DataBackLength = n * 8;
				}

				if (n == 0) {
					n = 1;
				}
				if (n > RC522_MAX_LENGTH) {
					n = RC522_MAX_LENGTH;
				}
				//Reading received data in FIFO
				for (count  = 0; count  < n; count++ ) {
					databack[count] = RC522ReadRegister(FIFODATAREG);
				}
//				databack[count] = 0;
			}
		}
		else {
			_status = MI_ERR;
		}
	}
	return _status;
}

uint8_t RC522Request(uint8_t requestmode, uint8_t *TagType){
	uint8_t _status;
	uint16_t backbits;  //The received data bits

	RC522WriteRegister(BITFRAMINGREG, 0x07); //Txlastbits = bitframingreg[2..0]

	TagType[0] = requestmode;
	_status = RC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backbits);

	if ((_status != MI_OK) || (backbits != 0x10)) {
		_status = MI_ERR;
	}
	return _status;
}

void RC522CRC(uint8_t *DataIn, uint8_t length, uint8_t *DataOut){
	uint8_t i, n;

	RC522ClearBit(DIVIRQREG, 0x04);	  //CRCIrq = 0
	RC522SetBit(FIFOLEVELREG, 0x80);  //clear FIFO pointer

	for (i = 0; i < length; i++) {
		RC522WriteRegister(FIFODATAREG, *(DataIn + i));
	}
	RC522WriteRegister(COMMANDREG, PCD_CALCCRC);

	i = 0xFF;
	do {
		n = RC522ReadRegister(DIVIRQREG);
		i --;
	} while (i &&  !(n & 0x04));
	DataOut[0] = RC522ReadRegister(CRCRESULTREGL);
	DataOut[1] = RC522ReadRegister(CRCRESULTREGM);
}

uint8_t RC522SelectTag(uint8_t *SerialNumber){
	uint8_t i;
	RC522Status_t _status;
	uint8_t size;
	uint16_t receivebits;
	uint8_t buffer[9];

	buffer[0] = PICC_SElECTTAG;
	buffer[1] = 0x70;

	for (i = 2; i < 7; i++) {
		buffer[i] = *SerialNumber++;
	}
	RC522CRC(buffer, 7, &buffer[7]);
	_status = RC522ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &receivebits);

	if ((_status == MI_OK) && (receivebits == 0x18)) {
		size = buffer[0];
	}
	else {
		size = 0;
	}
	return size;
}

void RC522Halt(){
	uint16_t unlength;
	uint8_t buffer[4];
	buffer[0] = PICC_HALT;
	buffer[1] = 0;
	RC522CRC(buffer, 2, &buffer[2]);
//	RC522ClearBit(STATUS2REG, 0x80);
	RC522ToCard(PCD_TRANSCEIVE, buffer, 4, buffer, &unlength);
//	RC522ClearBit(STATUS2REG, 0x08);
}

RC522Status_t RC522Authentication(uint8_t authmode, uint8_t blockaddress, uint8_t *SectorKey, uint8_t *SerialNumber){
	RC522Status_t _status;
	uint16_t receivebits;
	uint8_t i;
	uint8_t buffer[12];

	//Verify the command block address + sector + password + card serial number
	buffer[0] = authmode;
	buffer[1] = blockaddress;

	for (i = 2; i < 8; i++) {
		buffer[i] = SectorKey[i-2];
	}

	for (i = 8; i < 12; i++) {
		buffer[i] = SerialNumber[i-8];
	}

	_status = RC522ToCard(PCD_AUTHENT, buffer, 12, buffer, &receivebits);

	if ((_status != MI_OK) || !(RC522ReadRegister(STATUS2REG) & 0x08)) {
		_status = MI_ERR;
	}

	return _status;
}

RC522Status_t RC522BlockWrite(uint8_t blockaddress, uint8_t *WriteData){
	RC522Status_t _status;
	uint16_t receivebits;
	uint8_t i;
	uint8_t buffer[18];
	buffer[0] = PICC_WRITE;
	buffer[1] = blockaddress;

	RC522CRC(buffer, 2, &buffer[2]);
	_status = RC522ToCard(PCD_TRANSCEIVE, buffer, 4, buffer, &receivebits);

	if ((_status != MI_OK) || (receivebits != 4) || ((buffer[0] & 0x0F) != 0x0A)) {
		_status = MI_ERR;
	}
	if (_status == MI_OK) {
		for (i = 0; i < 16; i++) {
			buffer[i] = WriteData[i];
		}

		RC522CRC(buffer, 16, &buffer[16]);
		_status = RC522ToCard(PCD_TRANSCEIVE, buffer, 18, buffer, &receivebits);
		if ((_status != MI_OK) || (receivebits != 4) || ((buffer[0] & 0x0F) != 0x0A)) {
			_status = MI_ERR;
		}
	}
	return _status;
}

RC522Status_t RC522BlockRead(uint8_t blockaddress, uint8_t *ReceiveData){
	RC522Status_t _status;
	uint16_t unlength;
	ReceiveData[0] = PICC_READ;
	ReceiveData[1] = blockaddress;

	RC522CRC(ReceiveData, 2, &ReceiveData[2]);
	_status = RC522ToCard(PCD_TRANSCEIVE, ReceiveData, 4, ReceiveData, &unlength);

	if ((_status != MI_OK) || (unlength != 0x90)) {
		_status = MI_ERR;
	}
	return _status;
}

RC522Status_t RC522AntiCollision(uint8_t *SerialNumber){
	RC522Status_t _status;
	uint8_t i;
	uint8_t serialnumbercheck;
	uint16_t unlength;

	RC522WriteRegister(BITFRAMINGREG, 0x00);
	SerialNumber[0] = PICC_ANTICOLL;
	SerialNumber[1] = 0x20;

	RC522ClearBit(STATUS2REG, 0x08);

	_status = RC522ToCard(PCD_TRANSCEIVE, SerialNumber, 2, SerialNumber, &unlength);

	if (_status == MI_OK) {
		for (i = 0; i < 4; i++) {
			serialnumbercheck ^= SerialNumber[i];
		}
		if (serialnumbercheck = SerialNumber[4]) {
			_status = MI_ERR;
		}
	}
	return _status;
}

bool RC522IsCard(uint8_t *TagType){
//	uint8_t string[RC522_MAX_LENGTH];
	uint8_t status;
	status = RC522Request(PICC_REQIDL, TagType);
	if (status == MI_OK) {
		return 1;
	} else {
		return 0;
	}
}

bool RC522ReadCardSerial(uint8_t *String){
	RC522Status_t _status;
	_status = RC522AntiCollision(String);
	String[5] = 0;
	if (_status == MI_OK) {
		return 1;
	} else {
		return 0;
	}
}

//static void RC522TimerTimeout(void){
//
//}
//
//static void RC522StopTimeout(void){
//	if (rc522TimerID != INVALID_TIMER_ID)
//		TIMER_UnregisterEvent(rc522TimerID);
//	rc522TimerID = INVALID_TIMER_ID;
//}
//
//static TIMER_ID RC522RunTimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime){
//	RC522StopTimeout();
//	rc522TimerID = TIMER_RegisterEvent(TimeoutCallback, msTime);
//	return rc522TimerID;
//}
