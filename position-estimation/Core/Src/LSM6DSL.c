/*
 * lsm6dsl.c
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

	// wait until enough samples are in the FIFO in a blocking way
	uint8_t lower_num, upper_num;
	uint16_t num;
	do {
		// read number
		lower_num = LSM6DSL_read_byte(hi2c, LSM6DSL_FIFO_STATUS1, &status);
		upper_num = LSM6DSL_read_byte(hi2c, LSM6DSL_FIFO_STATUS2, &status);

		// combine
		num = (uint16_t) ((lower_num) | (upper_num & 0x7) << 8);
	} while (num < 6);


	// read samples out of FIFO.
	// As the BDU and IF_INC bits of the CTRL3_C register are set, meaning the register address
	// is incremented automatically, we can read 12 bytes out of the lower FIFO register directly
	uint8_t raw_buffer[12];
	status = HAL_I2C_Mem_Read(hi2c, LSM6DSL_ADDRESS, LSM6DSL_FIFO_DATA_OUT_L, I2C_MEMADD_SIZE_8BIT, raw_buffer, 12, HAL_MAX_DELAY);

	// convert the samples into 16 bit signed integers
	int16_t gyro_x_raw, gyro_y_raw, gyro_z_raw;
	int16_t acc_x_raw, acc_y_raw, acc_z_raw;
	gyro_x_raw = (int16_t)((raw_buffer[1] << 8) | raw_buffer[0]);
	gyro_y_raw = (int16_t)((raw_buffer[3] << 8) | raw_buffer[2]);
	gyro_z_raw = (int16_t)((raw_buffer[5] << 8) | raw_buffer[4]);

	acc_x_raw = (int16_t)((raw_buffer[7] << 8) | raw_buffer[6]);
	acc_y_raw = (int16_t)((raw_buffer[9] << 8) | raw_buffer[8]);
	acc_z_raw = (int16_t)((raw_buffer[11] << 8) | raw_buffer[10]);

	// convert them into floating point format, assuming G_FS = 250 and LA_FS = 2
	gyro_data->x = 8.75e-3f*gyro_x_raw;
	gyro_data->y = 8.75e-3f*gyro_y_raw;
	gyro_data->z = 8.75e-3f*gyro_z_raw;

	acc_data->x = 6.1e-5f*acc_x_raw;
	acc_data->y = 6.1e-5f*acc_y_raw;
	acc_data->z = 6.1e-5f*acc_z_raw;

	//printf("gx: %.2f\t, gy: %.2f\t, gz: %.2f\t, ax: %.2f\t, ay: %.2f\t, az: %.2f\t, num: %d\n\r", gyro_data->x, gyro_data->y, gyro_data->z, acc_data->x, acc_data->y, acc_data->z, num);

	return status;
}

void LSM6DSL_init(I2C_HandleTypeDef* hi2c){
	HAL_StatusTypeDef status;

	// CTRL1_XL register
	uint8_t ctrl1_acc = 0;
	ctrl1_acc |= (ODR_52HZ << 4);
	ctrl1_acc |= (FS_ACC_2G << 2);
	// ctrl1_acc |= // INSERT LPF1_BW_SEL
	// ctrl1_acc |= // INSERT BW0_XL
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL1_ACC, &ctrl1_acc, &status);

	// CTRL2_G register
	uint8_t ctrl2_gyro = 0;
	ctrl2_gyro |= (ODR_52HZ << 4);
	ctrl2_gyro |= (FS_GYRO_250DPS << 2);
	ctrl2_gyro |= (FS_GYRO_OVR_NONE << 1);
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL2_GYRO, &ctrl2_gyro, &status);

	// CTRL3_C register
	uint8_t ctrl3_c = 0;
	ctrl3_c |= (BDU_ENABLE << 6);
	ctrl3_c |= (IF_INC_ENABLE << 2);
	// ctrl3_c |= // INSERT many more
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL3_C, &ctrl3_c, &status);

	// CTRL5_C register
	uint8_t ctrl5_c = 0;
	ctrl5_c |= (ROUNDING_GYRO_ACC << 5);
	// ctrl5_c |= // INSERT many more
	LSM6DSL_write_byte(hi2c, LSM6DSL_CTRL5_C, &ctrl5_c, &status);

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
	fifo_ctrl5 |= ODR_52HZ << 3;
	fifo_ctrl5 |= FIFO_MODE_CONTINUOUS;
	LSM6DSL_write_byte(hi2c, LSM6DSL_FIFO_CTRL5, &fifo_ctrl5, &status);

	// INT1_CTRL register
	uint8_t int1_ctrl = 0;
	// int1_ctrl |= // INSERT many others
	int1_ctrl |= INT1_FULL_FLAG << 5;
	int1_ctrl |= INT1_THRESHOLD_FLAG << 3;
	LSM6DSL_write_byte(hi2c, LSM6DSL_INT1_CTRL, &int1_ctrl, &status);
}
