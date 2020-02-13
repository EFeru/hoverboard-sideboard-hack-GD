/**
  * This file is part of the hoverboard-sideboard-hack project.
  *
  * Copyright (C) 2020-2021 Emanuel FERU <aerdronix@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Define to prevent recursive inclusion
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include "gd32f1x0.h"
#include "defines.h"


extern volatile int8_t    i2c_status;
extern volatile i2c_cmd		i2c_ReadWriteCmd;
extern volatile uint8_t		i2c_slaveAddress;
extern volatile uint8_t		i2c_regAddress;
extern volatile uint8_t*	i2c_txbuffer;
extern volatile uint8_t*  i2c_rxbuffer;
extern volatile uint8_t   i2c_nDABytes;
extern volatile  int8_t   i2c_nRABytes;

#ifdef AUX45_USE_I2C
extern volatile int8_t    i2c_aux_status;
extern volatile i2c_cmd		i2c_aux_ReadWriteCmd;
extern volatile uint8_t		i2c_aux_slaveAddress;
extern volatile uint8_t		i2c_aux_regAddress;
extern volatile uint8_t*	i2c_aux_txbuffer;
extern volatile uint8_t*  i2c_aux_rxbuffer;
extern volatile uint8_t   i2c_aux_nDABytes;
extern volatile  int8_t   i2c_aux_nRABytes;
#endif

/* general functions */
void consoleLog(char *message);
void toggle_led(uint32_t gpio_periph, uint32_t pin);
void intro_demo_led(uint32_t tDelay);

/* i2c write/read functions */
int8_t i2c_writeBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t i2c_writeByte (uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
int8_t i2c_writeBit  (uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t  data);
int8_t i2c_readBytes (uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t i2c_readByte  (uint8_t slaveAddr, uint8_t regAddr, uint8_t *data);
int8_t i2c_readBit   (uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);

#ifdef AUX45_USE_I2C
int8_t i2c_aux_writeBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t i2c_aux_readBytes (uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
#endif

#endif

