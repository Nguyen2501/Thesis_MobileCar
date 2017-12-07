/*
 * ADC.c
 *
 *  Created on: Oct 27, 2017
 *      Author: Nguyen
 */
#include "../include.h"
#include "ADC.h"
#define USE_TIMER1

static void IRDetectorISR(void);
static void IRTimerTimeout(void);
static void IrStoptimeout(void);
static TIMER_ID IrRuntimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime);

static TIMER_ID Ir_TimerID = INVALID_TIMER_ID;
void GetValue(uint32_t *adc_raw);

uint32_t raw_ADC[7];
uint32_t sensorstate;
uint32_t calib_white[7], calib_black[7];

void ADCInit(void){
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

		ADCHardwareOversampleConfigure(ADC0_BASE, 64);
		ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH4);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH5);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH6);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);

		ADCSequenceEnable(ADC0_BASE, 0);
		ADCIntRegister(ADC0_BASE, 0, &IRDetectorISR);
		ADCIntEnable(ADC0_BASE, 0);

#ifdef USE_TIMER1
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
		TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 1000);
		TimerIntRegister(TIMER1_BASE, TIMER_A, &IRTimerTimeout);
		IntEnable(INT_TIMER1A);
		TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
		TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
		TimerEnable(TIMER1_BASE, TIMER_A);
#endif

#ifndef USE_TIMER1
		IrRuntimeout(&IRTimerTimeout, 1);
#endif
}

void IRInit(void){
	SysCtlPeripheralEnable(ADC0_BASE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);
}

void GetValue(uint32_t *adc_raw){
	ADCIntClear(ADC0_BASE, 0);
	ADCProcessorTrigger(ADC0_BASE, 0);
	while(!ADCIntStatus(ADC0_BASE, 0, false)){}
	ADCSequenceDataGet(ADC0_BASE, 0, adc_raw);
}

uint8_t LineState(){
	uint8_t sensorstate;
	int sensorcounter;
	ADCIntClear(ADC0_BASE, 0);
	ADCProcessorTrigger(ADC0_BASE, 0);
	while(!ADCIntStatus(ADC0_BASE, 0, false)){}
	ADCSequenceDataGet(ADC0_BASE, 0, raw_ADC);

	for (sensorcounter = 0; sensorcounter < 7; sensorcounter++) {
		if (raw_ADC[sensorcounter] < (calib_white[sensorcounter] - DELTA)) {
			sensorstate |= 1 << sensorcounter;  //Black
		} else {
			sensorstate &= ~(1 << sensorcounter);  //White
		}
	}
	return sensorstate;
}

int8_t LineValuePID(uint8_t sensorstate){
	switch (sensorstate) {
		case 0x40:
			return -6;
		case 0x60:
			return -5;
		case 0x20:
			return -4;
		case 0x30:
			return -3;
		case 0x10:
			return -2;
		case 0x18:
			return -1;
		case 0x08:
			return 0;
		case 0x0D:
			return 1;
		case 0x04:
			return 2;
		case 0x06:
			return 3;
		case 0x02:
			return 4;
		case 0x03:
			return 5;
		case 0x01:
			return 6;
		default:
			return 0;
	}
}

static void IRDetectorISR(void){
//	volatile uint32_t ADCResult[7];
	uint32_t ADCResult[7];
	uint8_t sensorcounter;
	ADCIntClear(ADC0_BASE, 0);
	ADCProcessorTrigger(ADC0_BASE, 0);
	while(!ADCIntStatus(ADC0_BASE, 0, false)){}
	ADCSequenceDataGet(ADC0_BASE, 0, ADCResult);
		for (sensorcounter = 0; sensorcounter < 7; sensorcounter++) {
			if (ADCResult[sensorcounter] < (calib_white[sensorcounter] - DELTA)) {
				sensorstate |= (1 << sensorcounter);  //Black
			} else {
				sensorstate &= ~(1 << sensorcounter);  //White
			}
		}
#ifndef USE_TIMER1
		IrRuntimeout(&IRTimerTimeout, 1);
#endif
}

static void IRTimerTimeout(void){
#ifdef USE_TIMER1
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
#endif
	Ir_TimerID = INVALID_TIMER_ID;
}
static void IrStoptimeout(void){
	if (Ir_TimerID != INVALID_TIMER_ID) {
		TIMER_UnregisterEvent(Ir_TimerID);
		Ir_TimerID = INVALID_TIMER_ID;
	}
}

static TIMER_ID IrRuntimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime){
	IrStoptimeout();
	Ir_TimerID = TIMER_RegisterEvent(TimeoutCallback, msTime);
	return Ir_TimerID;
}
