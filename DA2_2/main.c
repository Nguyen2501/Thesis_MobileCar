/*
 * main.c
 */
#include "include.h"

//#include "driverlib/flash.h"
//#include "driverlib/eeprom.h"


void main(void) {
//	uint32_t pui32Data[2];
//	uint32_t pui32Read[2];
//	pui32Data[0] = 0x12345678;
//	pui32Data[1] = 0x56789abc;
//
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	SetSystemState(SYSTEM_RED_LED);
	ConfigSystem();
	Timer_Init();
	LedInit();
	Button_Init();

//	SysCtlPeripheralEnable(LED_PERIPHERAL);
//	GPIOPinTypeGPIOOutput(LED_PORT, BLUE_LED | RED_LED | GREEN_LED);
//	GPIOPinWrite(LED_PORT, BLUE_LED | RED_LED | GREEN_LED, 0x00);
//	SysCtlDelay(20000000);

//	ButtonRegisterCallback(BUTTON_LEFT, &ButtonLeftHandler);
	ButtonRegisterCallback(BUTTON_RIGHT, &ButtonRightHandler);
//	SysCtlPeripheralEnable(EEPROM_PERIPHERAL);
//	EEPROMInit();
//	EEPROMMassErase();
//	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));
//	EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));
//	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));

//	GPIOPinWrite(LED_PORT, BLUE_LED | RED_LED | GREEN_LED, 0x04);
//
//	FlashErase(0x1000);
//	FlashProgram(pui32Data, 0x1000, sizeof(pui32Data));
//	GPIOPinWrite(LED_PORT, BLUE_LED | RED_LED | GREEN_LED, 0x02);
//	SysCtlDelay(20000000);

	while(1){
		ProcessSystemState();
	}
}
