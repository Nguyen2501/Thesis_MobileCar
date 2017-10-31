/*
 * QEI.h
 *
 *  Created on: Sep 25, 2017
 *      Author: Nguyen
 */

#ifndef QEI_QEI_H_
#define QEI_QEI_H_

//* Private function prototype ----------------------------------------------*/
static void QEI1_VelocityIsr(void);

//* Private variables -------------------------------------------------------*/
int32_t Velocity;
static bool qei_velocity_timeout;
static int32_t qei_velocity = 0;

void qei_init(uint16_t ms_Timebase);
bool qei_getVelocity(int32_t *Velocity);
void qei_setPos(int32_t pos);
int32_t qei_getPos();



#endif /* QEI_QEI_H_ */
