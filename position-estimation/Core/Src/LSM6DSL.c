/*
 * LSM6DSL.c
 *
 *  Created on: 13.04.2026
 *      Author: gutal
 */

#include "lsm6dsl.h"
#include <stdio.h>

// PRE: ---
// POST: fills pData with the data contained in reg_address and returns the HAL_STATUS
uint8_t LSM6DSL_read_byte(I2C_HandleTypeDef* hi2c, uint8_t reg_address, HAL_StatusTypeDef* status) {
	uint8_t data;
	*status = HAL_I2C_Mem_Read(hi2c, LSM6DSL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
	return data;
}

void LSM6DSL_write_byte(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData, HAL_StatusTypeDef* status) {
	*status = HAL_I2C_Mem_Write(hi2c, LSM6DSL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, pData, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LSM6DSL_read_fifo(I2C_HandleTypeDef* hi2c, vec3_t* acc_data, vec3_t* gyro_data) {
	HAL_StatusTypeDef status;

	// read number of unread samples
	//uint16_t unread_samples = 0;
	//unread_samples = LSM6DL_read_byte(hi2c, LSM6DSL_FIFO_STATUS2, &status);
	//unread_samples = LSM6DL_read_byte(hi2c, LSM6DSL_FIFO_STATUS1, &status) | ((unread_samples & 0x7) << 8);

	// read fifo samples
	uint8_t lower_bits[6];
	uint8_t upper_bits[6];
	status = HAL_I2C_Mem_Read(hi2c, LSM6DSL_ADDRESS, LSM6DSL_FIFO_DATA_OUT_L, I2C_MEMADD_SIZE_8BIT, lower_bits, 6, HAL_MAX_DELAY);
	status = HAL_I2C_Mem_Read(hi2c, LSM6DSL_ADDRESS, LSM6DSL_FIFO_DATA_OUT_H, I2C_MEMADD_SIZE_8BIT, upper_bits, 6, HAL_MAX_DELAY);

	gyro_data->x = ((uint16_t)lower_bits[0]) | (((uint16_t)upper_bits[0]) << 8);
	gyro_data->y = ((uint16_t)lower_bits[1]) | (((uint16_t)upper_bits[1]) << 8);
	gyro_data->z = ((uint16_t)lower_bits[2]) | (((uint16_t)upper_bits[2]) << 8);

	acc_data->x = ((uint16_t)lower_bits[3]) | (((uint16_t)upper_bits[3]) << 8);
	acc_data->y = ((uint16_t)lower_bits[4]) | (((uint16_t)upper_bits[4]) << 8);
	acc_data->z = ((uint16_t)lower_bits[5]) | (((uint16_t)upper_bits[5]) << 8);


	printf("gx: %d, gy: %d, gz: %d, ax: %d, ay: %d, az: %d \n\r", gyro_pattern.x, gyro_pattern.y, gyro_pattern.z, acc_pattern.x, acc_pattern.y, acc_pattern.z);

	return status;
}

void LSM6DSL_init(I2C_HandleTypeDef* hi2c){
	HAL_StatusTypeDef status;

	// CTRL1_XL register
	uint8_t ctrl1_acc = 0;
	ctrl1_acc |= (ODR_208HZ << 4);
	ctrl1_acc |= (FS_ACC_2G << 2);
	//ctrl1_acc |= // INSERT LPF1_BW_SEL
	//ctrl1_acc |= // INSERT BW0_XL
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL1_ACC, &ctrl1_acc, &status);

	// CTRL2_G register
	uint8_t ctrl2_gyro = 0;
	ctrl2_gyro |= (ODR_208HZ << 4);
	ctrl2_gyro |= (FS_GYRO_250DPS << 2);
	ctrl2_gyro |= (FS_GYRO_OVR_NONE << 1);
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL2_GYRO, &ctrl2_gyro, &status);

	// CTRL3_C register
	uint8_t ctrl3_c = 0;
	ctrl3_c |= (BDU_ENABLE << 6);
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL3_C, &ctrl3_c, &status);

	// FIFO_CTRL1 register
	uint8_t fifo_ctrl1 = (FIFO_THRESHOLD & 0xff); // last 8 bits (last significant) of the fifo threashold value
	LSM6DSL_write_byte(hi2c, LSM6DSL_FIFO_CTRL1, &fifo_ctrl1, &status);

	// FIFO_CTRL2 register
	uint8_t fifo_ctrl2 = 0;
	// fifo_ctrl2 |= // INSERT TIMER_PEDO_FIFO_EN
	// fifo_ctrl2 |= // INSERT TIMER_PEDO_FIFO_DRDY
	// fifo_ctrl2 |= // INSERT FIFO_TEMP_ENABLE
	fifo_ctrl2 |= (FIFO_THRESHOLD >> 8); // first 3 bits (most significant) of the fifo threashold value
	LSM6DSL_write_byte(hi2c, LSM6DSL_FIFO_CTRL2, &fifo_ctrl2, &status);

	// FIFO_CTRL3 register
	uint8_t fifo_ctrl3 = 0;
	fifo_ctrl3 |= FIFO_NO_DECIMATION << 3;
	fifo_ctrl3 |= FIFO_NO_DECIMATION;
	LSM6DSL_write_byte(hi2c, LSM6DSL_FIFO_CTRL3, &fifo_ctrl3, &status);

	// FIFO_CTRL4 register
	uint8_t fifo_ctrl4 = 0;
	fifo_ctrl4 |= FIFO_THRESHOLD_DISABLE << 7;
	// fifo_ctrl4 |= // INSERT ONLY_HIGH_DATA
	fifo_ctrl4 |= FIFO_DISABLE_DATASET << 3;
	fifo_ctrl4 |= FIFO_DISABLE_DATASET;
	LSM6DSL_write_byte(hi2c, LSM6DSL_FIFO_CTRL4, &fifo_ctrl4, &status);

	// FIFO_CTRL5 register
	uint8_t fifo_ctrl5 = 0;
	fifo_ctrl5 |= ODR_208HZ << 3;
	fifo_ctrl5 |= FIFO_MODE_CONTINUOUS;
	LSM6DSL_write_byte(hi2c, LSM6DSL_FIFO_CTRL5, &fifo_ctrl5, &status);

	// INT1_CTRL register
	uint8_t int1_ctrl = 0;
	// int1_ctrl |= // INSERT many others
	int1_ctrl |= INT1_FULL_FLAG << 5;
	int1_ctrl |= INT1_THRESHOLD_FLAG << 3;
	LSM6DSL_write_byte(hi2c, LSM6DSL_INT1_CTRL, &int1_ctrl, &status);
}
