/**
 * @file i2c.h
 * @brief 基于GPIO模拟iic
 * @author Tangent (498339337@qq.com)
 * @date 2022-04-08
 * 
 * @copyright Copyright (c) 2022 @Tangent
 */

#ifndef INC_TYJ_I2C_HPP_
#define INC_TYJ_I2C_HPP_

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

void swi2c_init(void);

int16_t start_i2c(void);

int16_t write_i2c(uint16_t data);

int16_t read_i2c(uint16_t ack);

int16_t stop_i2c(void);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* INC_TYJ_I2C_HPP_ */

