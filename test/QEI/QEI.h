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

void QeiInit(uint16_t ms_Timebase);
bool QeiGetVelocity(bool select, int32_t *Velocity);
void QeiSetPositionLeft(int32_t position);
int32_t QeiGetPositionLeft();
void QeiSetPositionRight(int32_t position);
int32_t QeiGetPositionRight();


#endif /* QEI_QEI_H_ */
