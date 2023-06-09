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

/* Tx structure USART MAIN */
#ifdef SERIAL_CONTROL
typedef struct{
  uint16_t  start;
  int16_t   pitch;      // Angle
  int16_t   dPitch;     // Angle derivative
  int16_t   cmd1;       // RC Channel 1
  int16_t   cmd2;       // RC Channel 2
  uint16_t  sensors;    // RC Switches and Optical sideboard sensors
  uint16_t  checksum;
} SerialSideboard;
#endif

#ifdef PID_USART_CONTROL
typedef struct{
  uint16_t start;
  int16_t  cmd1;        //Steer by default
  int16_t  cmd2;        //Speed by default
  uint16_t checksum;
} UsartSideboard;
#endif

/* Rx structure USART MAIN */
#ifdef SERIAL_FEEDBACK
typedef struct{
  uint16_t  start;
  int16_t   cmd1;
  int16_t   cmd2;
  int16_t   speedR_meas;
  int16_t   speedL_meas;
  int16_t   batVoltage;
  int16_t   boardTemp;
  uint16_t  cmdLed;
  uint16_t  checksum;
} SerialFeedback;
#endif


/*
What does a PID controller have?
I/O:
  - INPUT ANGLE (int16_t)
  - INPUT SETPOINT(int16_t)
  - OUTPUT MOTOR SPEED(int16_t)
CONST:
  - Kp
  - Ki
  - Kd
  - Direction
  - Output lower/upper limits
  - On/Off (Can be combined with direction -1 0 1)
  - Sample_time (1ms by default)
INTERNAL:
  - P,I,D components (each is a product of all coefficients) 
  - Error (Input-Setpoint) 
  - I accumulator
  - I limiter
  - D last position
CALCULATION:
function pid_compute(PID_CONTROLLER * pid)
  Output_tmp = (err * Kp * dir) + (I_accum * Ki * dir) + ((input - last_err) * Kd * dir)

  if |Output not in limits then clamp it

  I_accum += err*sample_time
  if I_accum more than limits then clamp, otherwise proceed
  last_err = input
*/
typedef struct{
  //Inputs / Outputs
  float input, setpoint, output;
  int16_t motor_output;
  //Constants
  float Kp, Ki, Kd;
  float outputLimit, I_limit;
  int16_t dir, sampleTime;
  //Internal variables (only modified by pid controller)
  float P, I, D;
  float err;
  float I_accumulator;
  float last_err;
} PID_CONTROLLER;

typedef PID_CONTROLLER * PTR_PID_CONTROLLER;




/* Tx structure USART AUX */

#ifdef SERIAL_AUX_TX
typedef struct{
  uint16_t  start;
  int16_t   signal1;
  int16_t   signal2;
  uint16_t  checksum;
} SerialAuxTx;
#endif
/* Rx structure USART AUX */
#ifdef SERIAL_AUX_RX
  #ifdef CONTROL_IBUS
  typedef struct{
    uint8_t  start;
    uint8_t  type; 
    uint8_t  channels[IBUS_NUM_CHANNELS*2];
    uint8_t  checksuml;
    uint8_t  checksumh;
  } SerialCommand;
  #endif
#endif

/* general functions */
void consoleLog(char *message);
void toggle_led(uint32_t gpio_periph, uint32_t pin);
void intro_demo_led(uint32_t tDelay);
uint8_t switch_check(uint16_t ch, uint8_t type);


/* PID related functions */
void pid_compute(PTR_PID_CONTROLLER pid);
void pid_init(PTR_PID_CONTROLLER pid,
              float P_k,
              float I_k,
              float D_k,
              float setP,
              float out_lim,
              float I_lim, int16_t direction, int16_t sample_t);
void pid_read_mpu_angle(PTR_PID_CONTROLLER pid);
void pid_get_motor_output(PTR_PID_CONTROLLER pid, int16_t *output);
void pid_log_output(PTR_PID_CONTROLLER pid);
void pid_set_sample_time(PTR_PID_CONTROLLER pid, uint16_t stime);


/* input initialization function */
void input_init(void);

/* handle functions */
void handle_mpu6050(void);
void handle_sensors(void);
void handle_usart(void);
void pid_handle_usart(PTR_PID_CONTROLLER pid);
void handle_leds(void);

/* usart1 read functions */
void usart1_rx_check(void);
#ifdef SERIAL_DEBUG
void usart_process_debug(uint8_t *userCommand, uint32_t len);
#endif
#ifdef SERIAL_FEEDBACK
void usart_process_data(SerialFeedback *Feedback_in, SerialFeedback *Feedback_out);
#endif

/* usart0 read functions */
void usart0_rx_check(void);
#ifdef SERIAL_AUX_RX
void usart_process_command(SerialCommand *command_in, SerialCommand *command_out);
#endif

/* AUX Serial Print data */
void aux_print_to_console(void);

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

