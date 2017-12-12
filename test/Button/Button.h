/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *  version 0.0.1
 */

/**
 * @file	Button.h
 * @brief	Push button driver
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

#define BUTTON_DEBOUNCE_MS		20

#define GPIO_PORT_BUTTON GPIO_PORTF_BASE
#define INT_BUTTON INT_GPIOF
#define SYSCTL_PERIPH_BUTTON SYSCTL_PERIPH_GPIOF
#define GPIO_PIN_BUTTON_LEFT GPIO_PIN_4
#define GPIO_PIN_BUTTON_RIGHT GPIO_PIN_0

typedef enum
{
	BUTTON_NONE = 0,
	BUTTON_LEFT,
	BUTTON_RIGHT
} BUTTON_TYPE;

extern void ButtonInit(void);
extern bool ButtonRegisterCallback(BUTTON_TYPE ButtonSelect, void (*ButtonCallback)());

#endif /* BUTTON_BUTTON_H_ */
