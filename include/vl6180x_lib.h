/**********************************************************************************************************************
 * \file vl6180x_lib.h
 *
 * \author  Marco Aguilar
 *
 * \date Jan-30-2024
 *
 * \version 1.0 \n \n
 *
 * This file contains:
 *********************************************************************************************************************/

#ifndef VL6180X_LIB_H
#define VL6180X_LIB_H

#include <stm32l4xx_hal.h>
#include <stm32l4xx_hal_i2c.h>
#include <stdint.h>

#define VL6180_ADDR 0x29
#define SYS_MOD_GPIO0_ADDR 0x10
#define RANGE_START_ADDR 0x18
#define RANGE_VAL_ADDR 0x62
#define RES_RANGE_STAT_ADDR 0x4D

#ifdef __cplusplus
extern "C" {
#endif

void vl6180_v_ReadData(I2C_HandleTypeDef* p_i2c, uint16_t u_addr, uint8_t* pu_readMsg, uint8_t u_size);

void vl6180_v_WriteData(I2C_HandleTypeDef* p_i2c, uint16_t u_addr, uint8_t* pu_writeMsg, uint8_t u_size);

#ifdef __cplusplus
}
#endif

#endif
