/*
 * LSM6DSL.h
 *
 *  Created on: 13.04.2026
 *      Author: gutal
 */


#ifndef SRC_LSM6DSL_H_
#define SRC_LSM6DSL_H_

#include "stm32l4xx_hal.h"

// register addresses
#define LSM6DSL_ADDRESS (0x6a << 1)
#define LSM6DSL_WHO_AM_I 0xf
#define LSM6DSL_CTRL1_ACC 0x10
#define LSM6DSL_CTRL2_GYRO 0x11

#define LSM6DSL_OUTX_L_GYRO 0x22
#define LSM6DSL_OUTX_H_GYRO 0x23
#define LSM6DSL_OUTY_L_GYRO 0x24
#define LSM6DSL_OUTY_H_GYRO 0x25
#define LSM6DSL_OUTZ_L_GYRO 0x26
#define LSM6DSL_OUTZ_H_GYRO 0x27

#define LSM6DSL_OUTX_L_ACC 0x28
#define LSM6DSL_OUTX_H_ACC 0x29
#define LSM6DSL_OUTY_L_ACC 0x2a
#define LSM6DSL_OUTY_H_ACC 0x2b
#define LSM6DSL_OUTZ_L_ACC 0x2c
#define LSM6DSL_OUTZ_H_ACC 0x2d

#define LSM6DSL_FIFO_CTRL1 0x6
#define LSM6DSL_FIFO_CTRL2 0x7
#define LSM6DSL_FIFO_CTRL3 0x8
#define LSM6DSL_FIFO_CTRL4 0x9
#define LSM6DSL_FIFO_CTRL5 0xa

#define LSM6DSL_INT1_CTRL 0xd

// values
#define ODR_POWER_DOWN 0x0
#define ODR_208HZ 0x5
#define ODR_416HZ 0x6

#define FS_ACC_2G 0x0
#define FS_ACC_4G 0x2
#define FS_ACC_8G 0x3
#define FS_ACC_16G 0x1

#define FS_GYRO_OVR_NONE 0x0
#define FS_GYRO_OVR_125DPS 0x1
#define FS_GYRO_250DPS 0x0
#define FS_GYRO_500DPS 0x1
#define FS_GYRO_1000DPS 0x2
#define FS_GYRO_2000DPS 0x3

#define FIFO_THRESHOLD 0x100
#define FIFO_THRESHOLD_ENABLE 0x1
#define FIFO_DISABLE_DATASET 0x0
#define FIFO_NO_DECIMATION 0x1
#define FIFO_MODE_CONTINUOUS 0x6

#define INT1_FULL_FLAG 0x1
#define INT1_THRESHOLD_FLAG 0x1

// DEIFNE BANDWIDTH LPF1_BW_SEL AND BW0_XL

typedef struct

HAL_StatusTypeDef LSM6DSL_read_byte(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData);
HAL_StatusTypeDef LSM6DSL_write_byte(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData);
//HAL_StatusTypeDef LSM6DSL_read_acc(I2C_HandleTypeDef* hi2c, uint8_t* pData);


void LSM6DSL_init();


#endif /* SRC_LSM6DSL_H_ */
