/*
 * LSM6DSL.c
 *
 *  Created on: 13.04.2026
 *      Author: gutal
 */

#include "LSM6DSL.h"

// PRE: ---
// POST: fills pData with the data contained in reg_address and returns the HAL_STATUS
HAL_StatusTypeDef LSM6DSL_read(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData){
	return HAL_I2C_Mem_Read(hi2c, LSM6DSL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, pData, 1, HAL_MAX_DELAY);
}


HAL_StatusTypeDef LSM6DSL_write(I2C_HandleTypeDef* hi2c, uint8_t reg_address, uint8_t* pData){
	return HAL_I2C_Mem_Write(hi2c, LSM6DSL_ADDRESS, reg_address, I2C_MEMADD_SIZE_8BIT, pData, 1, HAL_MAX_DELAY);
}

void LSM6DSL_init(){
	uint8_t ctrl1_acc = (LSM6DSL_ODR_ACC_208HZ << 4);
	ctrl1_acc |= (LSM6DSL_FS_ACC_2G << 3);
	// INSERT BANDWIDTH!!

}
