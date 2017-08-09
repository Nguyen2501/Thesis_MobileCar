/*
 * SENSOR_HMC5883L.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Nguyen
 */

#ifndef SENSOR_HMC5883L_SENSOR_HMC5883L_H_
#define SENSOR_HMC5883L_SENSOR_HMC5883L_H_

extern int headingAngle;
extern float headingOffset;

void hmc5883lInit(void);
void hmc5883lMeasurement(int16_t *xAxixvalue, int16_t *yAxixvalue, int16_t *zAxixvalue);

float hmc5883lCalibration(int xAxisvalue, int yAxisvalue, int zAxisvalue);
int hmc5883lAzimuth();

#endif /* SENSOR_HMC5883L_SENSOR_HMC5883L_H_ */
