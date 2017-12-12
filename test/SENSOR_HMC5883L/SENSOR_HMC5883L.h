/*
 * SENSOR_HMC5883L.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Nguyen
 */

#include "Kalman.h"
#ifndef SENSOR_HMC5883L_SENSOR_HMC5883L_H_
#define SENSOR_HMC5883L_SENSOR_HMC5883L_H_

#define SENSOR_PERIPHERAL SYSCTL_PERIPH_I2C1
#define SENSOR_ROOT_PERIPHERAL SYSCTL_PERIPH_GPIOA
#define SENSOR_PORT GPIO_PORTA_BASE
#define SENSOR_I2C	I2C1_BASE
#define SENSOR_GPIO_SCL GPIO_PA6_I2C1SCL
#define SENSOR_GPIO_SDA GPIO_PA7_I2C1SDA
#define SENSOR_PIN_SCL GPIO_PIN_6
#define SENSOR_PIN_SDA GPIO_PIN_7

extern void Hmc5883lInit(void);
extern void Hmc5883lMeasurement(int16_t *xAxixvalue, int16_t *yAxixvalue, int16_t *zAxixvalue);

extern float Hmc5883lCalibration(int xAxisvalue, int yAxisvalue, int zAxisvalue);
extern int Hmc5883lAzimuth();
void SetAngle(Kalman *KalmanStruct, float angle);
void SetQ(Kalman *KalmanStruct, float q);
void SetR(Kalman *KalmanStruct, float r);
float GetQ(Kalman *KalmanStruct);
float GetR(Kalman *KalmanStruct);
#endif /* SENSOR_HMC5883L_SENSOR_HMC5883L_H_ */
