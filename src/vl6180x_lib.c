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

#include <vl6180x_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

void vl6180_v_ReadData(I2C_HandleTypeDef* p_i2c, uint16_t u_addr, uint8_t* pu_readMsg, uint8_t u_size)
{
    uint8_t u_txBuffer[2] = {(uint8_t)(u_addr>>8),(uint8_t)u_addr};
    (void)HAL_I2C_Master_Transmit(p_i2c, (VL6180_ADDR<<1u), &u_txBuffer[0], 2u, 1000);
    (void)HAL_I2C_Master_Receive(p_i2c, (VL6180_ADDR<<1u), pu_readMsg, u_size, 1000);
}

void vl6180_v_WriteData(I2C_HandleTypeDef* p_i2c, uint16_t u_addr, uint8_t* pu_writeMsg, uint8_t u_size)
{
    uint8_t u_txBuffer[2+u_size];
    u_txBuffer[0] = (uint8_t)(u_addr>>8);
    u_txBuffer[1] = (uint8_t)(u_addr);
    u_txBuffer[2] = pu_writeMsg[0];
    (void)HAL_I2C_Master_Transmit(p_i2c, (VL6180_ADDR<<1u), &u_txBuffer[0], 2+u_size, 1000);
}

#ifdef __cplusplus
}
#endif
