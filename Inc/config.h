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
#ifndef CONFIG_H
#define CONFIG_H

/* ==================================== VARIANT SELECTION ==================================== */
// PlatformIO: uncomment desired variant in platformio.ini
// Keil uVision: select desired variant from the Target drop down menu (to the right of the Load button)
// Ubuntu: define the desired build variant here if you want to use make in console
// or use VARIANT environment variable for example like "make -e VARIANT=VARIANT_DEBUG". Select only one at a time.
#if !defined(PLATFORMIO)
  // #define VARIANT_DEBUG                       // Variant for debugging and checking the capabilities of the side-board
  // #define VARIANT_HOVERBOARD                  // Variant for using the side-boards connected to the Hoverboard mainboard
#endif

/* ==================================== DO NOT TOUCH SETTINGS ==================================== */
#define MPU6050                               // [-] Define IMU sensor type
#define MPU_GYRO_FSR              2000        // [deg/s] Set Gyroscope Full Scale Range: 250 deg/s, 500 deg/s, 1000 deg/s, 2000 deg/s. !! DMP sensor fusion works only with 2000 deg/s !!
#define MPU_ACCEL_FSR             2           // [g] Set Acceleromenter Full Scale Range: 2g, 4g, 8g, 16g. !! DMP sensor fusion works only with 2g !!
#define MPU_I2C_SPEED             400000      // [bit/s] Define I2C speed for communicating with the MPU6050
#define DELAY_IN_MAIN_LOOP        1           // [ms] Delay in the main loop
// #define PRINTF_FLOAT_SUPPORT                  // [-] Uncomment this for printf to support float on Serial Debug. It will increase code size! Better to avoid it!
/* =============================================================================================== */


/* ==================================== SETTINGS MPU-6050 ==================================== */
#define MPU_SENSOR_ENABLE                     // [-] Enable flag for MPU-6050 sensor. Comment-out this flag to Disable the MPU sensor and reduce code size.
#define MPU_DMP_ENABLE                        // [-] Enable flag for MPU-6050 DMP (Digital Motion Processing) functionality.
#define MPU_DEFAULT_HZ            20          // [Hz] Default MPU frequecy: must be between 1Hz and 200Hz.
#define TEMP_READ_MS              500         // [ms] Temperature read time interval
#define PEDO_READ_MS              1000        // [ms] Pedometer read time interval
// #define USE_CAL_HW_REGISTERS                  // [-] Uncommnent this to SAVE the sensor calibration to the MPU-6050 registers after the Self-test was run

// DMP Tap Detection Settings
#define DMP_TAP_AXES              TAP_XYZ     // [-] Set which axes will register a tap: TAP_XYZ, TAP_X, TAP_Y, TAP_Z
#define DMP_TAP_THRESH            250         // [mg/ms] Set tap threshold for the selected axis.
#define DMP_TAP_COUNT             1           // [-] Set minimum number of taps needed for an interrupt. Minimum consecutive taps: 1 to 4
#define DMP_TAP_TIME              100         // [ms] Set time length between valid taps.
#define DMP_TAP_TIME_MULTI        500         // [ms] Set max time between taps to register as a multi-tap.
#define DMP_SHAKE_REJECT_THRESH   200         // [deg/s] Set shake rejection threshold in degree per second (dps). If the DMP detects a gyro sample larger than the thresh, taps are rejected.
#define DMP_SHAKE_REJECT_TIME     40          // [ms] Set shake rejection time. Sets the length of time that the gyro must be outside of the DMP_SHAKE_REJECT_THRESH before taps are rejected. A mandatory 60 ms is added to this parameter.
#define DMP_SHAKE_REJECT_TIMEOUT  10          // [ms] Set shake rejection timeout. Sets the length of time after a shake rejection that the gyro must stay inside of the threshold before taps can be detected again. A mandatory 60 ms is added to this parameter.


/* ==================================== SETTINGS USART ==================================== */
#if defined(VARIANT_DEBUG)
  #define SERIAL_DEBUG                        // [-] Define for Serial Debug via the serial port
#elif defined(VARIANT_HOVERBOARD)
  #define SERIAL_CONTROL                      // [-] Define for Serial Control via the serial port
  #define SERIAL_FEEDBACK                     // [-] Define for Serial Feedback via the serial port
#endif
#define USART_MAIN_BAUD           38400       // [bit/s] MAIN Serial Tx/Rx baud rate
#define SERIAL_START_FRAME        0xABCD      // [-] Start frame definition for reliable serial communication
#define SERIAL_BUFFER_SIZE        64          // [bytes] Size of Serial Rx buffer. Make sure it is always larger than the 'Feedback' structure size
#define SERIAL_TIMEOUT            600         // [-] Number of wrong received data for Serial timeout detection. Depends on DELAY_IN_MAIN_LOOP


/* ==================================== SETTINGS AUX ==================================== */
// #define AUX45_USE_GPIO                        // [-] Use AUX4, AUX5 as GPIO ports
// #define AUX45_USE_I2C                         // [-] Use AUX4, AUX5 as I2C port
#define AUX45_USE_USART                       // [-] Use AUX4, AUX5 as USART port
#ifdef AUX45_USE_USART
  #define USART_AUX_BAUD          38400       // [bit/s] AUX Serial Tx/Rx baud rate
#endif
#ifdef AUX45_USE_I2C
  #define AUX_I2C_SPEED           100000      // [bit/s] Define I2C speed for communicating via AUX45 wires
#endif


/* ==================================== VALIDATE SETTINGS ==================================== */
#if defined(SERIAL_DEBUG) && defined(SERIAL_CONTROL)
  #error SERIAL_DEBUG and SERIAL_CONTROL not allowed. It is on the same cable.
#endif

#if defined(SERIAL_DEBUG) && defined(SERIAL_FEEDBACK)
  #error SERIAL_DEBUG and SERIAL_FEEDBACK not allowed. It is on the same cable.
#endif

#if defined(AUX45_USE_GPIO) && (defined(AUX45_USE_USART) || defined(AUX45_USE_I2C)) || (defined(AUX45_USE_USART) && defined(AUX45_USE_I2C))
  #error AUX45_USE_(GPIO,USART,I2C) not allowed in the same time. It is on the same cable.
#endif

#endif
