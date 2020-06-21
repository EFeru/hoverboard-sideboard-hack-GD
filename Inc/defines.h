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
#ifndef DEFINES_H
#define DEFINES_H


// Includes
#include "gd32f1x0.h"
#include "config.h"

#if defined(PRINTF_FLOAT_SUPPORT) && defined(SERIAL_DEBUG) && defined(__GNUC__)
	asm(".global _printf_float"); 		// this is the magic trick for printf to support float. Warning: It will increase code considerably! Better to avoid!
#endif

/* =========================== Defines General =========================== */
// #define _BV(bit) 					(1 << (bit)) 
#define ARRAY_LEN(x) 				(uint32_t)(sizeof(x) / sizeof(*(x)))
#define min(a, b) 					(((a) < (b)) ? (a) : (b))
#define max(a, b) 					(((a) > (b)) ? (a) : (b))
#define i2c_write   				i2c_writeBytes
#define i2c_read    				i2c_readBytes 
#define delay_ms    				delay_1ms
#define get_ms      				get_tick_count_ms
#define log_i       				printf			// redirect the log_i debug function to printf


/* =========================== Defines LEDs =========================== */
#define LED1_GPIO_Port 				GPIOA
#define LED1_Pin 					GPIO_PIN_0		// RED
#define LED2_GPIO_Port 				GPIOB
#define LED2_Pin 					GPIO_PIN_9		// GREEN
#define LED3_GPIO_Port 				GPIOB
#define LED3_Pin 					GPIO_PIN_8		// YELLOW
#define LED4_GPIO_Port 				GPIOB
#define LED4_Pin 					GPIO_PIN_5		// BLUE1
#define LED5_GPIO_Port 				GPIOB
#define LED5_Pin 					GPIO_PIN_4		// BLUE2

#define LED1_SET     				(0x01)
#define LED2_SET     				(0x02)
#define LED3_SET     				(0x04)
#define LED4_SET     				(0x08)
#define LED5_SET     				(0x10)


/* =========================== Defines SENSORS =========================== */
#define SENSOR1_GPIO_Port 			GPIOA
#define SENSOR1_Pin 				GPIO_PIN_4
#define SENSOR2_GPIO_Port 			GPIOC
#define SENSOR2_Pin 				GPIO_PIN_14


/* =========================== Defines USART =========================== */
#define USART_GPIO_PORT     		GPIOA
#define USART_GPIO_CLK      		RCU_GPIOA
#define USART_AF            		GPIO_AF_1

// USART ports number
#define USARTn              		2


// USART to Auxiliary, connected to USART0
#define USART_AUX           		USART0
#define USART_AUX_CLK       		RCU_USART0
#define USART_AUX_TX_PIN   			GPIO_PIN_9
#define USART_AUX_RX_PIN    		GPIO_PIN_10


// USART to Mainboard, connected to USART1
#define USART_MAIN     				USART1
#define USART_MAIN_CLK    			RCU_USART1
#define USART_MAIN_TX_PIN   		GPIO_PIN_2
#define USART_MAIN_RX_PIN   		GPIO_PIN_3

// USART address for DMA defines
#define USART0_TDATA_ADDRESS    	((uint32_t)0x40013828)  // USART0: 0x4001 3800 - 0x4001 3BFF
#define USART0_RDATA_ADDRESS    	((uint32_t)0x40013824)
#define USART1_TDATA_ADDRESS   	 	((uint32_t)0x40004428) 	// USART1: 0x4000 4400 - 0x4000 47FF
#define USART1_RDATA_ADDRESS    	((uint32_t)0x40004424)


/* =========================== Defines AUX =========================== */
#define AUX1_PU_GPIO_Port			GPIOC
#define AUX1_PU_Pin					GPIO_PIN_15
#define AUX2_GPIO_Port 				GPIOA
#define AUX2_Pin 					GPIO_PIN_1
#define AUX3_GPIO_Port 				GPIOB
#define AUX3_Pin 					GPIO_PIN_10
#ifdef AUX45_USE_GPIO
	#define AUX4_GPIO_Port 			GPIOA
	#define AUX4_Pin 				GPIO_PIN_10
	#define AUX5_GPIO_Port 			GPIOA
	#define AUX5_Pin 				GPIO_PIN_9
#endif


/* =========================== Defines I2C =========================== */
typedef enum {READ = 0, WRITE = !READ} i2c_cmd;
#define MPU_I2C 					I2C0
#define MPU_RCU_I2C					RCU_I2C0
#define MPU_SCL_GPIO_Port 			GPIOB
#define MPU_SCL_Pin 				GPIO_PIN_6
#define MPU_SDA_GPIO_Port 			GPIOB
#define MPU_SDA_Pin 				GPIO_PIN_7
#define I2C_OWN_ADDRESS7    		0x24

#ifdef AUX45_USE_I2C
	#define AUX_I2C 				I2C1
	#define AUX_RCU_I2C				RCU_I2C1
	#define AUX_SCL_GPIO_Port 		GPIOA
	#define AUX_SCL_Pin 			GPIO_PIN_9
	#define AUX_SDA_GPIO_Port 		GPIOA
	#define AUX_SDA_Pin 			GPIO_PIN_10
	#define AUX_I2C_OWN_ADDRESS7 	0x34
#endif

/* =========================== Defines MPU-6050 =========================== */
#define RAD2DEG 					57.295779513082323  	// RAD2DEG = 180/pi. Example: angle[deg] = angle[rad] * RAD2DEG
#define q30 						1073741824  			// 1073741824 = 2^30
#define ACCEL_ON        			(0x01)
#define GYRO_ON         			(0x02)
#define COMPASS_ON      			(0x04)

#define PRINT_ACCEL     			(0x01)
#define PRINT_GYRO      			(0x02)
#define PRINT_QUAT      			(0x04)
#define PRINT_EULER     			(0x08)
#define PRINT_TEMP     				(0x10)
#define PRINT_PEDO      			(0x20)

typedef struct{
	int16_t  	x;
	int16_t  	y;
	int16_t  	z; 
} Gyro;

typedef struct{
	int16_t  	x;
	int16_t  	y;
	int16_t  	z; 
} Accel;

typedef struct{
	int32_t  	w;
	int32_t  	x;
	int32_t  	y;
	int32_t  	z; 
} Quaternion;

typedef struct{
	int16_t  	roll;
	int16_t  	pitch;
	int16_t  	yaw;
} Euler;

typedef struct {
	Gyro 		gyro;
	Accel 		accel;
	Quaternion 	quat;
	Euler 		euler;
	int16_t 	temp;
} MPU_Data;

#endif
