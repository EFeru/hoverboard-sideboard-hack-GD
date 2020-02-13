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

// Includes
#include <stdio.h>
#include "systick.h"
#include "gd32f1x0.h"
#include "defines.h"
#include "config.h"
#include "util.h"


// MAIN I2C variables
volatile int8_t     i2c_status;
volatile i2c_cmd	i2c_ReadWriteCmd;
volatile uint8_t	i2c_regAddress;
volatile uint8_t	i2c_slaveAddress;
volatile uint8_t*   i2c_txbuffer;
volatile uint8_t*   i2c_rxbuffer;
volatile uint8_t    i2c_nDABytes;
volatile  int8_t    i2c_nRABytes;
volatile uint8_t 	buffer[14];

#ifdef AUX45_USE_I2C
// AUX I2C variables
volatile int8_t     i2c_aux_status;
volatile i2c_cmd	i2c_aux_ReadWriteCmd;
volatile uint8_t	i2c_aux_regAddress;
volatile uint8_t	i2c_aux_slaveAddress;
volatile uint8_t*   i2c_aux_txbuffer;
volatile uint8_t*   i2c_aux_rxbuffer;
volatile uint8_t    i2c_aux_nDABytes;
volatile  int8_t    i2c_aux_nRABytes;
#endif


/* =========================== General Functions =========================== */

void consoleLog(char *message)
{
  #ifdef SERIAL_DEBUG
	log_i("%s", message);
  #endif
}


/* retarget the C library printf function to the USART */
#ifdef SERIAL_DEBUG	
	#ifdef __GNUC__
		#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	#else
		#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#endif
	PUTCHAR_PROTOTYPE {
		usart_data_transmit(USART_MAIN, (uint8_t)ch);
		while(RESET == usart_flag_get(USART_MAIN, USART_FLAG_TBE));
		return ch;
	}
	
	#ifdef __GNUC__
		int _write(int file, char *data, int len) {
			int i;
			for (i = 0; i < len; i++) { __io_putchar( *data++ );}
			return len; 
		}
	#endif
#endif


void toggle_led(uint32_t gpio_periph, uint32_t pin)
{
	GPIO_OCTL(gpio_periph) ^= pin;
}

void intro_demo_led(uint32_t tDelay)
{
	int i;

	for (i = 0; i < 6; i++) {
		gpio_bit_set(LED1_GPIO_Port, LED1_Pin);
		gpio_bit_reset(LED3_GPIO_Port, LED3_Pin);			
		delay_1ms(tDelay);
		gpio_bit_set(LED2_GPIO_Port, LED2_Pin);
		gpio_bit_reset(LED1_GPIO_Port, LED1_Pin);
		delay_1ms(tDelay);
		gpio_bit_set(LED3_GPIO_Port, LED3_Pin);
		gpio_bit_reset(LED2_GPIO_Port, LED2_Pin);
		delay_1ms(tDelay);
	}
	
	for (i = 0; i < 2; i++) {
		gpio_bit_set(LED1_GPIO_Port, LED1_Pin);
		gpio_bit_set(LED2_GPIO_Port, LED2_Pin);
		gpio_bit_set(LED3_GPIO_Port, LED3_Pin);
		gpio_bit_set(LED4_GPIO_Port, LED4_Pin);
		gpio_bit_set(LED5_GPIO_Port, LED5_Pin);
		delay_1ms(tDelay);
		gpio_bit_reset(LED1_GPIO_Port, LED1_Pin);
		gpio_bit_reset(LED2_GPIO_Port, LED2_Pin);
		gpio_bit_reset(LED3_GPIO_Port, LED3_Pin);
		gpio_bit_reset(LED4_GPIO_Port, LED4_Pin);
		gpio_bit_reset(LED5_GPIO_Port, LED5_Pin);
	}		
		
}

/* =========================== I2C WRITE Functions =========================== */

/*
 * write bytes to chip register
 */
int8_t i2c_writeBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{	
	
	// assign WRITE command
	i2c_ReadWriteCmd 	= WRITE;			
	
	// assign inputs	
	i2c_status 			= -1;
	i2c_slaveAddress 	= slaveAddr << 1;		// Address is shifted one position to the left. LSB is reserved for the Read/Write bit.
	i2c_regAddress 		= regAddr;	
	i2c_txbuffer 		= data;
	i2c_nDABytes 		= length;
	i2c_nRABytes 		= 1;
	
	// enable the I2C0 interrupt
	i2c_interrupt_enable(MPU_I2C, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);

	// the master waits until the I2C bus is idle
	while(i2c_flag_get(MPU_I2C, I2C_FLAG_I2CBSY));

	// the master sends a start condition to I2C bus
	i2c_start_on_bus(MPU_I2C);

	// Wait until all data bytes are sent/received
	while(i2c_nDABytes > 0);
	
	return i2c_status;
	
}


/*
 * write 1 byte to chip register
 */
int8_t i2c_writeByte(uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
	return i2c_writeBytes(slaveAddr, regAddr, 1, &data);
}


/*
 * write one bit to chip register
 */
int8_t i2c_writeBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    i2c_readByte(slaveAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return i2c_writeByte(slaveAddr, regAddr, b);
}



/* =========================== I2C READ Functions =========================== */

/*
 * read bytes from chip register
 */
int8_t i2c_readBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data) 
{
	
	// assign READ command
	i2c_ReadWriteCmd 	= READ;
	
	// assign inputs
	i2c_status 			= -1;
	i2c_slaveAddress 	= slaveAddr << 1; 	// Address is shifted one position to the left. LSB is reserved for the Read/Write bit.
	i2c_regAddress 		= regAddr;
	i2c_rxbuffer 		= data;
	i2c_nDABytes 		= length;
	i2c_nRABytes 		= 1;   
    
	// enable the I2C0 interrupt
	i2c_interrupt_enable(MPU_I2C, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);

	if(2 == i2c_nDABytes){			
			i2c_ackpos_config(MPU_I2C, I2C_ACKPOS_NEXT);			// send ACK for the next byte
	}
	
	// the master waits until the I2C bus is idle
	while(i2c_flag_get(MPU_I2C, I2C_FLAG_I2CBSY));
	
	// the master sends a start condition to I2C bus
	i2c_start_on_bus(MPU_I2C);
	
	// Wait until all data bytes are sent/received
	while(i2c_nDABytes > 0);
	
	// Return status
	return i2c_status;
	
}


/*
 * read 1 byte from chip register
 */
int8_t i2c_readByte(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data)
{
	return i2c_readBytes(slaveAddr, regAddr, 1, data);
}


/*
 * read 1 bit from chip register
 */
int8_t i2c_readBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data)
{
    uint8_t b;
    int8_t status = i2c_readByte(slaveAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return status;
}


#ifdef AUX45_USE_I2C
/*
 * write bytes to chip register
 */
int8_t i2c_aux_writeBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{	
	
	// assign WRITE command
	i2c_aux_ReadWriteCmd 	= WRITE;			
	
	// assign inputs	
	i2c_aux_status 			= -1;
	i2c_aux_slaveAddress 	= slaveAddr << 1; 	// Address is shifted one position to the left. LSB is reserved for the Read/Write bit.
	i2c_aux_regAddress 		= regAddr;	
	i2c_aux_txbuffer 		= data;
	i2c_aux_nDABytes 		= length;
	i2c_aux_nRABytes 		= 1;
	
	// enable the I2C0 interrupt
	i2c_interrupt_enable(AUX_I2C, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);

	// the master waits until the I2C bus is idle
	while(i2c_flag_get(AUX_I2C, I2C_FLAG_I2CBSY));

	// the master sends a start condition to I2C bus
	i2c_start_on_bus(AUX_I2C);

	// Wait until all data bytes are sent/received
	while(i2c_aux_nDABytes > 0);
	
	return i2c_aux_status;
	
}

/*
 * read bytes from chip register
 */
int8_t i2c_aux_readBytes(uint8_t slaveAddr, uint8_t regAddr, uint8_t length, uint8_t *data) 
{
	
	// assign READ command
	i2c_aux_ReadWriteCmd 	= READ;
	
	// assign inputs
	i2c_aux_status 			= -1;
	i2c_aux_slaveAddress 	= slaveAddr << 1;			// Address is shifted one position to the left. LSB is reserved for the Read/Write bit.
	i2c_aux_regAddress 		= regAddr;
	i2c_aux_rxbuffer 		= data;
	i2c_aux_nDABytes 		= length;
	i2c_aux_nRABytes 		= 1;   
    
	// enable the I2C0 interrupt
	i2c_interrupt_enable(AUX_I2C, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);

	if(2 == i2c_aux_nDABytes){			
			i2c_ackpos_config(AUX_I2C, I2C_ACKPOS_NEXT);			// send ACK for the next byte
	}
	
	// the master waits until the I2C bus is idle
	while(i2c_flag_get(AUX_I2C, I2C_FLAG_I2CBSY));
	
	// the master sends a start condition to I2C bus
	i2c_start_on_bus(AUX_I2C);
	
	// Wait until all data bytes are sent/received
	while(i2c_aux_nDABytes > 0);
	
	// Return status
	return i2c_aux_status;
	
}
#endif





