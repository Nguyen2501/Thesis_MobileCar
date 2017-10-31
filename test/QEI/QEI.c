/*
 * QEI.c
 *
 *  Created on: Sep 25, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "QEI.h"

void qei_init(uint16_t ms_Timebase)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET
    		| QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP, 0xFFFFFFFF);
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PC5_PHA1);
    GPIOPinConfigure(GPIO_PC6_PHB1);
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_2, SysCtlClockGet() * ms_Timebase/ 1000);
    QEIVelocityEnable(QEI1_BASE);
    QEIEnable(QEI1_BASE);

    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);

    QEIIntRegister(QEI1_BASE, &QEI1_VelocityIsr);

    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
}

static void QEI1_VelocityIsr(void){
	QEIIntClear(QEI1_BASE, QEIIntStatus(QEI1_BASE, true));
	qei_velocity = QEIVelocityGet(QEI1_BASE) * QEIDirectionGet(QEI1_BASE);
	qei_velocity_timeout = true;
}

bool qei_getVelocity(int32_t *Velocity){
	if (qei_velocity_timeout) {
		*Velocity = qei_velocity;
		qei_velocity_timeout = false;
		return true;
	}
	else
		return false;
}

int32_t qei_getPos()
{
	return QEIPositionGet(QEI1_BASE);
}

void qei_setPos(int32_t pos)
{
	QEIPositionSet(QEI1_BASE,pos);
}


