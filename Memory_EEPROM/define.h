/*
 * define.h
 *
 *  Created on: Jul 31, 2017
 *      Author: Doggy-Nguyen
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#define BUTTON_PERIPHERAL SYSCTL_PERIPH_GPIOF
#define INT_BUTTON INT_GPIOF
#define BUTTON_PORT GPIO_PORTF_BASE
#define LEFT_BUTTON GPIO_PIN_4
#define RIGHT_BUTTON GPIO_PIN_0

#define LED_PERIPHERAL SYSCTL_PERIPH_GPIOF
#define EEPROM_PERIPHERAL SYSCTL_PERIPH_EEPROM0
#define LED_PORT GPIO_PORTF_BASE
#define BLUE_LED GPIO_PIN_1
#define RED_LED GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

#define BLUE_LED_ON() GPIOPinWrite(LED_PORT, BLUE_LED, BLUE_LED)
#define BLUE_LED_OFF() GPIOPinWrite(LED_PORT, BLUE_LED, 0x00)
#define BLUE_LED_TOGGLE() GPIOPinWrite(LED_PORT, BLUE_LED, ~GPIOPinRead(LED_PORT,BLUE_LED))

#define GREEN_LED_ON() GPIOPinWrite(LED_PORT, GREEN_LED, GREEN_LED)
#define GREEN_LED_OFF() GPIOPinWrite(LED_PORT, GREEN_LED, 0x00)
#define GREEN_LED_TOGGLE() GPIOPinWrite(LED_PORT, GREEN_LED, ~GPIOPinRead(LED_PORT,GREEN_LED))

#define RED_LED_ON() GPIOPinWrite(LED_PORT, RED_LED, RED_LED)
#define RED_LED_OFF() GPIOPinWrite(LED_PORT, RED_LED, 0x00)
#define RED_LED_TOGGLE() GPIOPinWrite(LED_PORT, RED_LED, ~GPIOPinRead(LED_PORT,RED_LED))

#define LEFT_BUTTON_ON (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4))
#define RIGHT_BUTTON_ON (!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))




#endif /* DEFINE_H_ */
