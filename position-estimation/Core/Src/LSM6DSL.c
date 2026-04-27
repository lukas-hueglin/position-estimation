/*
 * LSM6DSL.c
 *
 *  Created on: 13.04.2026
 *      Author: gutal
 */

#include "lsm6dsl.h"

// PRE: ---
// POST: fills pData with the data contained in reg_address and returns the HAL_STATUS
HAL_StatusTypeDef LSM6DSL_read_byte(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData){
	return HAL_I2C_Mem_Read(hi2c, LSM6DSL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, pData, 1, HAL_MAX_DELAY);
}


HAL_StatusTypeDef LSM6DSL_write_byte(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData){
	return HAL_I2C_Mem_Write(hi2c, LSM6DSL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, pData, 1, HAL_MAX_DELAY);
}

void LSM6DSL_init(I2C_HandleTypeDef* hi2c){

	// CTRL1_XL register
	uint8_t ctrl1_acc = 0;
	ctrl1_acc |= (ODR_208HZ << 4);
	ctrl1_acc |= (FS_ACC_2G << 2);
	//ctrl1_acc |= // INSERT LPF1_BW_SEL
	//ctrl1_acc |= // INSERT BW0_XL
	LSM6DSL_write(hi2c, LSM6DSL_CTRL1_ACC, &ctrl1_acc);

	// CTRL2_G register
	uint8_t ctrl2_gyro = 0;
	ctrl2_gyro |= (ODR_208HZ << 4);
	ctrl2_gyro |= (FS_GYRO_250DPS << 2);
	ctrl2_gyro |= (FS_GYRO_OVR_NONE << 1);
	LSM6DSL_write(hi2c, LSM6DSL_CTRL2_GYRO, &ctrl2_gyro);

	// FIFO_CTRL1 register
	uint8_t fifo_ctrl1 = (FIFO_THRESHOLD & 0xff); // last 8 bits (last significant) of the fifo threashold value
	LSM6DSL_write(hi2c, LSM6DSL_FIFO_CTRL1, &fifo_ctrl1);

	// FIFO_CTRL2 register
	uint8_t fifo_ctrl2 = 0;
	// fifo_ctrl2 |= // INSERT TIMER_PEDO_FIFO_EN
	// fifo_ctrl2 |= // INSERT TIMER_PEDO_FIFO_DRDY
	// fifo_ctrl2 |= // INSERT FIFO_TEMP_ENABLE
	fifo_ctrl2 |= (FIFO_THRESHOLD >> 8); // first 3 bits (most significant) of the fifo threashold value
	LSM6DSL_write(hi2c, LSM6DSL_FIFO_CTRL2, &fifo_ctrl2);

	// FIFO_CTRL3 register
	uint8_t fifo_ctrl3 = 0;
	fifo_ctrl3 |= FIFO_NO_DECIMATION << 3;
	fifo_ctrl3 |= FIFO_NO_DECIMATION;
	LSM6DSL_write(hi2c, LSM6DSL_FIFO_CTRL3, &fifo_ctrl3);

	// FIFO_CTRL4 register
	uint8_t fifo_ctrl4 = 0;
	fifo_ctrl4 |= FIFO_THRESHOLD_ENABLE << 7;
	// fifo_ctrl4 |= // INSERT ONLY_HIGH_DATA
	fifo_ctrl4 |= FIFO_DISABLE_DATASET << 3;
	fifo_ctrl4 |= FIFO_DISABLE_DATASET;
	LSM6DSL_write(hi2c, LSM6DSL_FIFO_CTRL4, &fifo_ctrl4);

	// FIFO_CTRL5 register
	uint8_t fifo_ctrl5 = 0;
	fifo_ctrl5 |= ODR_416HZ << 3;
	fifo_ctrl5 |= FIFO_MODE_CONTINUOUS;
	LSM6DSL_write(hi2c, LSM6DSL_FIFO_CTRL5, &fifo_ctrl5);

	// INT1_CTRL register
	uint8_t int1_ctrl = 0;
	// int1_ctrl |= // INSERT many others
	int1_ctrl |= INT1_FULL_FLAG << 5;
	int1_ctrl |= INT1_THRESHOLD_FLAG << 3;
	LSM6DSL_write(hi2c, LSM6DSL_INT1_CTRL, &int1_ctrl);
}
