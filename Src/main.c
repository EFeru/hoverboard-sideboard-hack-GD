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

#ifdef SERIAL_CONTROL
typedef struct{
	uint16_t	start;
	int16_t  	roll;
	int16_t  	pitch;
	int16_t  	yaw;
	uint16_t  	sensors;
	uint16_t 	checksum;
} SerialSideboard;
SerialSideboard Sideboard;
#endif

#ifdef SERIAL_FEEDBACK
typedef struct{
	uint16_t 	start;
	int16_t 	cmd1;
	int16_t 	cmd2;
	int16_t 	speedR;
	int16_t 	speedL;
	int16_t 	speedR_meas;
	int16_t 	speedL_meas;
	int16_t 	batVoltage;
	int16_t 	boardTemp;
	uint16_t 	cmdLed;
	uint16_t  	checksum;
} SerialFeedback;
SerialFeedback Feedback;
SerialFeedback NewFeedback;

static int16_t timeoutCntSerial   = 0;  				// Timeout counter for Rx Serial command
static uint8_t timeoutFlagSerial  = 0;  				// Timeout Flag for Rx Serial command: 0 = OK, 1 = Problem detected (line disconnected or wrong Rx data)
#endif

extern MPU_Data 	mpu;								// holds the MPU-6050 data
ErrStatus 			mpuStatus = SUCCESS;				// holds the MPU-6050 status: SUCCESS or ERROR

uint8_t 			userCommand;						// holds the user command input
FlagStatus 			sensor1, sensor2; 					// holds the sensor1 and sensor 2 values
FlagStatus			sensor1_read, sensor2_read;			// holds the instantaneous Read for sensor1 and sensor 2

static uint32_t 	main_loop_counter;					// main loop counter to perform task squeduling inside main()


int main(void)
{		
	systick_config(); 									// SysTick config    
	gpio_config();										// GPIO config		
	usart_config(USART_MAIN, USART_MAIN_BAUD); 			// USART config
	i2c_config(); 										// I2C config			
	i2c_nvic_config();									// NVIC peripheral config	
	
	#ifdef SERIAL_CONTROL
		usart_Tx_DMA_config(USART_MAIN, (uint8_t *)&Sideboard, 		sizeof(Sideboard));
	#endif
	#ifdef SERIAL_FEEDBACK
		usart_Rx_DMA_config(USART_MAIN, (uint8_t *)&NewFeedback,  sizeof(NewFeedback));
	#endif
	
	intro_demo_led(100);								// Short LEDs intro demo with 100 ms delay. This also gives some time for the MPU-6050 to power-up.	
	if(mpu_config()) { 									// IMU MPU-6050 config
		mpuStatus = ERROR;
		gpio_bit_set(LED1_GPIO_Port, LED1_Pin);			// Turn on RED LED
	}
	else {
		gpio_bit_set(LED2_GPIO_Port, LED2_Pin);			// Turn on GREEN LED
	}
	mpu_handle_input('h'); 								// Print the User Help commands to serial

	while(1) {
		
		delay_1ms(DELAY_IN_MAIN_LOOP);
		
		// ==================================== LEDs Handling ====================================
		// toggle_led(LED4_GPIO_Port, LED4_Pin); 		// Toggle BLUE1 LED
		#ifdef SERIAL_FEEDBACK
			if (!timeoutFlagSerial) {
				if (Feedback.cmdLed & LED1_SET) { gpio_bit_set(LED1_GPIO_Port, LED1_Pin); } else { gpio_bit_reset(LED1_GPIO_Port, LED1_Pin); }
				if (Feedback.cmdLed & LED2_SET) { gpio_bit_set(LED2_GPIO_Port, LED2_Pin); } else { gpio_bit_reset(LED2_GPIO_Port, LED2_Pin); }
				if (Feedback.cmdLed & LED3_SET) { gpio_bit_set(LED3_GPIO_Port, LED3_Pin); } else { gpio_bit_reset(LED3_GPIO_Port, LED3_Pin); }
				if (Feedback.cmdLed & LED4_SET) { gpio_bit_set(LED4_GPIO_Port, LED4_Pin); } else { gpio_bit_reset(LED4_GPIO_Port, LED4_Pin); }
				if (Feedback.cmdLed & LED5_SET) { gpio_bit_set(LED5_GPIO_Port, LED5_Pin); } else { gpio_bit_reset(LED5_GPIO_Port, LED5_Pin); }
			}
		#endif
	

		// ==================================== USER Handling ====================================
		#ifdef SERIAL_DEBUG
		// Get the user Input as one character from Serial
			if(SET == usart_flag_get(USART_MAIN, USART_FLAG_RBNE)) { 	//  Check if Read Buffer Not Empty meanind Serial data is available
					userCommand = usart_data_receive(USART_MAIN);
					if (userCommand != 10 && userCommand != 13) { 		// Do not accept 'new line' (ascii 10) and 'carriage return' (ascii 13) commands
							log_i("Command = %c\n", userCommand);						
							mpu_handle_input(userCommand);
					}
			}
		#endif
			
		
		// ==================================== MPU-6050 Handling ====================================
		// Get MPU data. Because the MPU-6050 interrupt pin is not wired we have to check DMP data by pooling periodically
		if (SUCCESS == mpuStatus) {
			mpu_get_data();
		}
		// Print MPU data to Console
		if (main_loop_counter % 50 == 0) {
			mpu_print_to_console();
		}	
		

		// ==================================== SENSORS Handling ====================================
		sensor1_read = gpio_input_bit_get(SENSOR1_GPIO_Port, SENSOR1_Pin);
		sensor2_read = gpio_input_bit_get(SENSOR2_GPIO_Port, SENSOR2_Pin);

		// SENSOR1
		if (sensor1 == RESET && sensor1_read == SET) {
			sensor1 = SET;
			// Sensor ACTIVE: Do something here (one time task on activation)
			gpio_bit_set(LED4_GPIO_Port, LED4_Pin);
			consoleLog("-- SENSOR 1 Active --\n");		
		} else if(sensor1 == SET && sensor1_read == RESET) {
			sensor1 = RESET;
			gpio_bit_reset(LED4_GPIO_Port, LED4_Pin);			
		}
		
		// SENSOR2
		if (sensor2 == RESET && sensor2_read == SET) {			
			sensor2 = SET;
			// Sensor ACTIVE: Do something here (one time task on activation)
			gpio_bit_set(LED5_GPIO_Port, LED5_Pin);
		} else if (sensor2 == SET && sensor2_read == RESET) {
			sensor2 = RESET;
			gpio_bit_reset(LED5_GPIO_Port, LED5_Pin);
		}

		if (sensor1 == SET) {
			// Sensor ACTIVE: Do something here (continuous task)
		}
		if (sensor2 == SET) {
			// Sensor ACTIVE: Do something here (continuous task)
		}
		
		
		// ==================================== SERIAL Tx/Rx Handling ====================================
		#ifdef SERIAL_CONTROL
			// To transmit on USART
			if (main_loop_counter % 50 == 0 && SET == dma_flag_get(DMA_CH3, DMA_FLAG_FTF)) {		//  check if DMA channel transfer complete (Full Transfer Finish flag == 1)
				
				Sideboard.start    	= (uint16_t)SERIAL_START_FRAME;
				Sideboard.roll    	= (int16_t)mpu.euler.roll;
				Sideboard.pitch    	= (int16_t)mpu.euler.pitch;
				Sideboard.yaw    	= (int16_t)mpu.euler.yaw;
				Sideboard.sensors	= (uint16_t)(sensor1 | (sensor2 << 1));
				Sideboard.checksum 	= (uint16_t)(Sideboard.start ^ Sideboard.roll ^ Sideboard.pitch ^ Sideboard.yaw ^ Sideboard.sensors);
			
				dma_channel_disable(DMA_CH3);
				DMA_CHCNT(DMA_CH3) 		= sizeof(Sideboard);
				DMA_CHMADDR(DMA_CH3) 	= (uint32_t)&Sideboard;
				dma_channel_enable(DMA_CH3);		
			}
		#endif
		
		#ifdef SERIAL_FEEDBACK
			uint16_t checksum;
			checksum = (uint16_t)(NewFeedback.start ^ NewFeedback.cmd1 ^ NewFeedback.cmd2 ^ NewFeedback.speedR ^ NewFeedback.speedL
								^ NewFeedback.speedR_meas ^ NewFeedback.speedL_meas ^ NewFeedback.batVoltage ^ NewFeedback.boardTemp ^ NewFeedback.cmdLed);
			if (NewFeedback.start == SERIAL_START_FRAME && NewFeedback.checksum == checksum) {
			if (timeoutFlagSerial) {                    	// Check for previous timeout flag  
			if (timeoutCntSerial-- <= 0)              		// Timeout de-qualification
				timeoutFlagSerial = 0;                  	// Timeout flag cleared           
			} else {
				memcpy(&Feedback, &NewFeedback, sizeof(SerialFeedback));	// Copy the new data 
				NewFeedback.start = 0xFFFF;             	// Change the Start Frame for timeout detection in the next cycle
				timeoutCntSerial  = 0;                  	// Reset the timeout counter         
          	}
			} else {
				if (timeoutCntSerial++ >= SERIAL_TIMEOUT) { // Timeout qualification
				timeoutFlagSerial = 1;                      // Timeout detected
				timeoutCntSerial  = SERIAL_TIMEOUT;         // Limit timout counter value
				}
				// Check periodically the received Start Frame. If it is NOT OK, most probably we are out-of-sync. Try to re-sync by reseting the DMA
				if (main_loop_counter % 50 == 0 && NewFeedback.start != SERIAL_START_FRAME && NewFeedback.start != 0xFFFF) {
					dma_channel_disable(DMA_CH4);            
					usart_Rx_DMA_config(USART_MAIN, (uint8_t *)&NewFeedback,  sizeof(NewFeedback));
				}
			}
						
			if (timeoutFlagSerial) {                   		// In case of timeout bring the system to a Safe State and indicate error if desired
				gpio_bit_set(LED1_GPIO_Port, LED1_Pin);		// Turn on Red LED
			} else {
				gpio_bit_reset(LED1_GPIO_Port, LED1_Pin);   // Follow the Normal behavior
			}		
		#endif	
		
		
		main_loop_counter++;
		
	}
}


