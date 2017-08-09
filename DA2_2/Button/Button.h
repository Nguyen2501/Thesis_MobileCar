/*
 * Button.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Nguyen
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

typedef enum
{
	BUTTON_NONE = 0,
	BUTTON_LEFT,
	BUTTON_RIGHT
} BUTTON_TYPE;

#define BUTTON_DEBOUNCE_MS		20

extern void Button_Init(void);
extern bool ButtonRegisterCallback(BUTTON_TYPE ButtonSelect, void (*ButtonCallback)());
extern void ButtonLeftHandler(void);
extern void ButtonRightHandler(void);

#endif /* BUTTON_BUTTON_H_ */
