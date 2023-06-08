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

#include <stdio.h>
#include <string.h>
#include "gd32f1x0.h"
#include "systick.h"
#include "i2c_it.h"
#include "defines.h"
#include "setup.h"
#include "config.h"
#include "util.h"
#include "mpu6050.h"
#include "mpu6050_dmp.h"

uint32_t     main_loop_counter;                         // main loop counter to perform task scheduling inside main()
PID_CONTROLLER pid_data;

int main(void)
{
    systick_config();                                   // SysTick config
    gpio_config();                                      // GPIO config

    usart_config(USART_MAIN, USART_MAIN_BAUD);          // USART MAIN config
    #if defined(SERIAL_AUX_RX) || defined(SERIAL_AUX_TX)
    usart_config(USART_AUX,  USART_AUX_BAUD);           // USART AUX config
    #endif
    usart_nvic_config();                                // USART interrupt configuration

    i2c_config();                                       // I2C config
    i2c_nvic_config();                                  // I2C interrupt configuration
    input_init();                                       // Input initialization
    pid_init(&pid_data, def_kp, def_ki, def_kd, def_setpoint, def_outputLimit, def_I_limit, def_sampleTime, def_direction);

    while(1) {

        delay_1ms(DELAY_IN_MAIN_LOOP);

        pid_read_mpu_angle(&pid_data);
        pid_compute(&pid_data);
        pid_log_output(&pid_data);
        

        handle_mpu6050();                               // Handle of the MPU-6050 IMU sensor
        handle_sensors();                               // Handle of the optical sensors
        #ifdef CONTROL_PID
          pid_handle_usart(&pid_data);
        #else
          handle_usart();                                 // Handle of the USART data
        #endif
        handle_leds();                                  // Handle of the sideboard LEDs

        main_loop_counter++;

    }
}


