/*
 * LSM6DSL.h
 *
 *  Created on: 13.04.2026
 *      Author: gutal
 */


#ifndef SRC_LSM6DSL_H_
#define SRC_LSM6DSL_H_

#include "stm32l4xx_hal.h"

#define LSM6DSL_ADDRESS (0x6A << 1)
#define LSM6DSL_WHO_AM_I 0x0f


#define LSM6DSL_GYR_OUTX_L_G 0x22
#define LSM6DSL_GYR_OUTX_H_G 0x23
#define LSM6DSL_GYR_OUTY_L_G 0x24
#define LSM6DSL_GYR_OUTY_H_G 0x25
#define LSM6DSL_GYR_OUTZ_L_G 0x26
#define LSM6DSL_GYR_OUTZ_H_G 0x27

#define LSM6DSL_ACC_OUTX_L_ACC 0x28
#define LSM6DSL_ACC_OUTX_H_ACC 0x29
#define LSM6DSL_ACC_OUTY_L_ACC 0x2A
#define LSM6DSL_ACC_OUTY_H_ACC 0x2B
#define LSM6DSL_ACC_OUTZ_L_ACC 0x2C
#define LSM6DSL_ACC_OUTZ_H_ACC 0x2D


#define LSM6DSL_ODR_ACC_POWER_DOWN 0x0
#define LSM6DSL_ODR_ACC_208HZ 0x5
#define LSM6DSL_FS_ACC_2G 0x0
#define LSM6DSL_FS_ACC_4G 0x2
#define LSM6DSL_FS_ACC_8G 0x3
#define LSM6DSL_FS_ACC_16G 0x1
// DEIFNE BANDWIDTH LPF1_BW_SEL AND BW0_XL

HAL_StatusTypeDef LSM6DSL_read(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData);
HAL_StatusTypeDef LSM6DSL_write(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData);

void LSM6DSL_init();


#endif /* SRC_LSM6DSL_H_ */
