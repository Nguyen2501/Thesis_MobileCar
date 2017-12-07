/*
 * ADC.h
 *
 *  Created on: Oct 27, 2017
 *      Author: Nguyen
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#define DELTA 80

extern void ADCInit(void);
extern void IRInit(void);
extern uint8_t LineState(void);
extern int8_t LineValuePID(uint8_t sensorstate);
#endif /* ADC_ADC_H_ */
