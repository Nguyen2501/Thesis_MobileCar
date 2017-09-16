/*
 * main.c
 */
#include "include.h"
uint8_t tagtype;
uint8_t UID[6];
uint8_t status;
uint8_t string[RC522_MAX_LENGTH];
extern bool RC522IsCard(uint8_t *TagType);
void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Enable UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}

void main(void) {

	RC522Init();
	ConfigureUART();
	UARTprintf("Hello");
	while(1){
	status = RC522Request(PICC_REQIDL, string);
//	uint8_t string[RC522_MAX_LENGTH];
	if (status == MI_OK) {
		UARTprintf("Card detected :");
	}

	status = RC522AntiCollision(string);
	if (status) {
		UARTprintf("[%02x-%02x-%02x-%02x-%02x]", string[0], string[1], string[2], string[3], string[4]);
		UARTprintf("\n\r");
	}
	SysCtlDelay(1000);
	RC522Halt();
	}

}

