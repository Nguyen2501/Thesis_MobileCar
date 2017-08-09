/*
 * HMC5883LPinMap.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Nguyen
 */

#ifndef SENSOR_HMC5883L_HMC5883LPINMAP_H_
#define SENSOR_HMC5883L_HMC5883LPINMAP_H_


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup CoX_Driver_Lib
//! @{
//! \addtogroup Sensor
//! @{
//! \addtogroup Digital_Compass
//! @{
//! \addtogroup HMC5883L
//! @{
//
//*****************************************************************************


//*****************************************************************************
//
//! \addtogroup HMC5883L Register description
//! @{
//
//*****************************************************************************

//!Address define
#define HMC5883L_ADD  0x1e
//! Bit define, used in this file only. not for USER
#define BIT_8_0                 ((uint8_t) 0x01)
#define BIT_8_1                 ((uint8_t) 0x02)
#define BIT_8_2                 ((uint8_t) 0x04)
#define BIT_8_3                 ((uint8_t) 0x08)
#define BIT_8_4                 ((uint8_t) 0x10)
#define BIT_8_5                 ((uint8_t) 0x20)
#define BIT_8_6                 ((uint8_t) 0x40)
#define BIT_8_7                 ((uint8_t) 0x80)

//! Register Address
//Configuration Register A   Read/Write
#define HMC5883L_CFG_A          ((uint8_t) 0x00)

//Configuration Register B   Read/Write
#define HMC5883L_CFG_B          ((uint8_t) 0x01)

//Mode Register  Read/Write
#define HMC5883L_MODE           ((uint8_t) 0x02)

//Data register    Read-only
#define HMC5883L_DATA_X_MSB     ((uint8_t) 0x03)
#define HMC5883L_DATA_X_LSB     ((uint8_t) 0x04)
#define HMC5883L_DATA_Z_MSB     ((uint8_t) 0x05)
#define HMC5883L_DATA_Z_LSB     ((uint8_t) 0x06)
#define HMC5883L_DATA_Y_MSB     ((uint8_t) 0x07)
#define HMC5883L_DATA_Y_LSB     ((uint8_t) 0x08)

//Status register Read-only
#define HMC5883L_STATUS         ((uint8_t) 0x09)

//Identification Register
#define HMC5883L_ID_A           ((uint8_t) 0x0A)
#define HMC5883L_ID_B           ((uint8_t) 0x0B)
#define HMC5883L_ID_C           ((uint8_t) 0x0C)

//! Register Bit define
#define CFG_A_MA_1              BIT_8_6
#define CFG_A_MA_0              BIT_8_5

#define CFG_A_DO_2              BIT_8_4
#define CFG_A_DO_1              BIT_8_3
#define CFG_A_DO_0              BIT_8_2

#define CFG_A_MS_1              BIT_8_1
#define CFG_A_MS_0              BIT_8_0

#define CFG_B_GN_2              BIT_8_7
#define CFG_B_GN_1              BIT_8_6
#define CFG_B_GN_0              BIT_8_5

#define MODE_MD_1               BIT_8_1
#define MODE_MD_0               BIT_8_0

#define STATUS_LOCK             BIT_8_1
#define STATUS_RDY              BIT_8_0

#define ID_A_VALUE              ((uint8_t)0x48)
#define ID_B_VALUE              ((uint8_t)0x34)
#define ID_C_VALUE              ((uint8_t)0x33)


//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! @}
//! @}
//! @}
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
//! Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif /* SENSOR_HMC5883L_HMC5883LPINMAP_H_ */
