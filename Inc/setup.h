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
#ifndef SETUP_H
#define SETUP_H

// Includes
#include "gd32f1x0.h"

// Function declarations
void gpio_config(void);
void usart_config(uint32_t selUSART, uint32_t selBaudRate);
void usart_Tx_DMA_config(uint32_t selUSART, uint8_t *pData, uint32_t Size);
void usart_Rx_DMA_config(uint32_t selUSART, uint8_t *pData, uint32_t Size);

void i2c_config(void);
void i2c_nvic_config(void);

#endif

