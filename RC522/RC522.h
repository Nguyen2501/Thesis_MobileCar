/*
 * RC522.h
 *
 *  Created on: Aug 28, 2017
 *      Author: Nguyen
 */

#ifndef RC522_H_
#define RC522_H_


#define RC522_FUNCTION SYSCTL_PERIPH_SSI2
#define RC522_ALTERNATE_FUNCTION SYSCTL_PERIPH_GPIOB
#define RC522_ROOT GPIO_PORTB_BASE
#define RC522_PORT SSI2_BASE
#define RC522_SCK_PIN GPIO_PIN_4
#define RC522_CS_PIN GPIO_PIN_5
#define RC522_MISO_PIN GPIO_PIN_6
#define RC522_MOSI_PIN GPIO_PIN_7
#define RC522_CS_LOW() GPIOPinWrite(RC522_ROOT, RC522_CS_PIN, 0)
#define RC522_CS_HIGH() GPIOPinWrite(RC522_ROOT, RC522_CS_PIN, RC522_CS_PIN)


#define RC522_RESET_FUNCTION SYSCTL_PERIPH_GPIOF
#define RC522_RESET_PORT GPIO_PORTF_BASE
#define RC522_RESET_PIN GPIO_PIN_4

//MF522 The error code is returned when communication
typedef enum {
	MI_OK = 0,
	MI_NOTAGERR,
	MI_ERR
} RC522Status_t;

//#use spi(FORCE_HW,master,baud = 10000000, BITS=8,MSB_FIRST, stream = RFID, ENABLE_ACTIVE=0)// uses hardware SPI and gives this stream the name SPI_STREAM
//MF522 Command word
#define PCD_IDLE              0x00               //NO action; Cancel the current command
#define PCD_AUTHENT           0x0E               //Authentication Key
#define PCD_RECEIVE           0x08               //Receive Data
#define PCD_TRANSMIT          0x04               //Transmit data
#define PCD_TRANSCEIVE        0x0C               //Transmit and receive data,
#define PCD_RESETPHASE        0x0F               //Reset
#define PCD_CALCCRC           0x03               //CRC Calculate
// Mifare_One card command word
#define PICC_REQIDL          0x26               // find the antenna area does not enter hibernation
#define PICC_REQALL          0x52               // find all the cards antenna area
#define PICC_ANTICOLL        0x93               // anti-collision
#define PICC_SElECTTAG       0x93               // election card
#define PICC_AUTHENT1A       0x60               // authentication key A
#define PICC_AUTHENT1B       0x61               // authentication key B
#define PICC_READ            0x30               // Read Block
#define PICC_WRITE           0xA0               // write block
#define PICC_DECREMENT       0xC0               // debit
#define PICC_INCREMENT       0xC1               // recharge
#define PICC_RESTORE         0xC2               // transfer block data to the buffer
#define PICC_TRANSFER        0xB0               // save the data in the buffer
#define PICC_HALT            0x50               // Sleep

//------------------MFRC522 Register---------------
//Page 0:Command and Status
#define     RESERVED00            0x00
#define     COMMANDREG            0x01
#define     COMMIENREG            0x02
#define     DIVLENREG             0x03
#define     COMMIRQREG            0x04
#define     DIVIRQREG             0x05
#define     ERRORREG              0x06
#define     STATUS1REG            0x07
#define     STATUS2REG            0x08
#define     FIFODATAREG           0x09
#define     FIFOLEVELREG          0x0A
#define     WATERLEVELREG         0x0B
#define     CONTROLREG            0x0C
#define     BITFRAMINGREG         0x0D
#define     COLLREG               0x0E
#define     RESERVED01            0x0F
//PAGE 1:Command
#define     RESERVED10            0x10
#define     MODEREG               0x11
#define     TXMODEREG             0x12
#define     RXMODEREG             0x13
#define     TXCONTROLREG          0x14
#define     TXAUTOREG             0x15
#define     TXSELREG              0x16
#define     RXSELREG              0x17
#define     RXTHRESHOLDREG        0x18
#define     DEMODREG              0x19
#define     RESERVED11            0x1A
#define     RESERVED12            0x1B
#define     MIFAREREG             0x1C
#define     RESERVED13            0x1D
#define     RESERVED14            0x1E
#define     SERIALSPEEDREG        0x1F
//PAGE 2:CFG
#define     RESERVED20            0x20
#define     CRCRESULTREGM         0x21
#define     CRCRESULTREGL         0x22
#define     RESERVED21            0x23
#define     MODWIDTHREG           0x24
#define     RESERVED22            0x25
#define     RFCFGREG              0x26
#define     GSNREG                0x27
#define     CWGSPREG              0x28
#define     MODGSPREG             0x29
#define     TMODEREG              0x2A
#define     TPRESCALERREG         0x2B
#define     TRELOADREGH           0x2C
#define     TRELOADREGL           0x2D
#define     TCOUNTERVALUEREGH     0x2E
#define     TCOUNTERVALUEREGL     0x2F
//PAGE 3:TEST REGISTER
#define     RESERVED30            0x30
#define     TESTSEL1REG           0x31
#define     TESTSEL2REG           0x32
#define     TESTPINENREG          0x33
#define     TESTPINVALUEREG       0x34
#define     TESTBUSREG            0x35
#define     AUTOTESTREG           0x36
#define     VERSIONREG            0x37
#define     ANALOGTESTREG         0x38
#define     TESTDAC1REG           0x39
#define     TESTDAC2REG           0x3A
#define     TESTADCREG            0x3B
#define     RESERVED31            0x3C
#define     RESERVED32            0x3D
#define     RESERVED33            0x3E
#define     RESERVED34            0x3F

#define RC522_MAX_LENGTH					16

extern void SPIInit();
extern uint8_t SSIReadWrite(uint8_t writedata);
extern void SSIWrite(uint8_t writedata);
extern void RC522WriteRegister(uint8_t address, uint8_t value);
extern uint8_t RC522ReadRegister(uint8_t address);

extern void RC522ClearBit(uint8_t address, uint8_t mask);
extern void RC522SetBit(uint8_t address, uint8_t mask);
extern void RC522Reset();
extern void RC522AntennaOn();
extern void RC522AntennaOff();
extern void RC522Init();
extern RC522Status_t RC522ToCard(uint8_t command, uint8_t *DataSend, uint8_t datasendlength, uint8_t *databack, uint16_t *DataBackLength);
extern uint8_t RC522Request(uint8_t requestmode, uint8_t *TagType);
extern void RC522CRC(uint8_t *DataIn, uint8_t length, uint8_t *DataOut);
extern uint8_t RC522SelectTag(uint8_t *SerialNumber);
extern void RC522Halt();
extern RC522Status_t RC522Authentication(uint8_t authmode, uint8_t blockaddress, uint8_t *SectorKey, uint8_t *SerialNumber);
extern RC522Status_t RC522BlockWrite(uint8_t blockaddress, uint8_t *WriteData);
extern RC522Status_t RC522BlockRead(uint8_t blockaddress, uint8_t *ReceiveData);
extern RC522Status_t RC522AntiCollision(uint8_t *SerialNumber);
bool RC22IsCard(uint8_t TagType);
extern bool RC522ReadCardSerial(uint8_t *String);

#endif /* RC522_H_ */
