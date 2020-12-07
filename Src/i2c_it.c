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


#include "gd32f1x0_i2c.h"
#include "i2c_it.h"
#include "systick.h"
#include "config.h"
#include "defines.h"

extern volatile int8_t    i2c_status;
extern volatile i2c_cmd   i2c_ReadWriteCmd;
extern volatile uint8_t   i2c_slaveAddress;
extern volatile uint8_t   i2c_regAddress;
extern volatile uint8_t*  i2c_txbuffer;
extern volatile uint8_t*  i2c_rxbuffer;
extern volatile uint8_t   i2c_nDABytes;
extern volatile  int8_t   i2c_nRABytes;

#ifdef AUX45_USE_I2C
extern volatile int8_t    i2c_aux_status;
extern volatile i2c_cmd   i2c_aux_ReadWriteCmd;
extern volatile uint8_t   i2c_aux_slaveAddress;
extern volatile uint8_t   i2c_aux_regAddress;
extern volatile uint8_t*  i2c_aux_txbuffer;
extern volatile uint8_t*  i2c_aux_rxbuffer;
extern volatile uint8_t   i2c_aux_nDABytes;
extern volatile  int8_t   i2c_aux_nRABytes;
#endif


void I2C0_EventIRQ_Handler(void)
{
    uint16_t k;
    if (i2c_ReadWriteCmd == WRITE) {        // check for WRITE command

        // ======================================== WRITE ========================================
        // --------------------------------------------------------------------
        // | Master | S | AD+W |     | RA |     | DATA |     | DATA |     | P |
        // | Slave  |   |      | ACK |    | ACK |      | ACK |      | ACK |   |
        // --------------------------------------------------------------------

        if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SBSEND)) {                            // check if start condition is sent out in master mode

            i2c_master_addressing(I2C0, i2c_slaveAddress, I2C_TRANSMITTER);                 // send slave address	with Transmit request

        } else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND)) {                    // check if address is sent in master mode

            i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);                           // clear ADDSEND bit

        } else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE)) {                        // check if I2C_DATA is empty (Transmitted Byte Empty)

            if (i2c_nRABytes > 0) {                                                         // check if the Register Address has been sent
                i2c_data_transmit(I2C0, i2c_regAddress);                                    // the master sends the Register Address byte
                i2c_nRABytes--;
            } else {
                if (i2c_nDABytes > 0) {
                    i2c_data_transmit(I2C0, *i2c_txbuffer++);                               // the master sends a data byte
                    i2c_nDABytes--;
                    if (0 == i2c_nDABytes) {
                        i2c_status = 0;                                                     // 0 = Success
                    }
                    for(k=0; k<500; k++) {
                        #ifdef __GNUC__
                            asm volatile ("nop");                                           // unoptimizable NOP loop, 500 times to make some clock cycles delay (otherwise DMP writing will fail!! Reason unknown yet.. could be that writing to MPU6050 memory takes a bit more time)
                        #else
                            __asm volatile ("nop");
                        #endif
                    }
                } else {
                    i2c_stop_on_bus(I2C0);                                                  // the master sends a stop condition to I2C bus
                    i2c_interrupt_disable(I2C0, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);    // disable the I2C0 interrupt
                }
            }

        }

    } else if (i2c_ReadWriteCmd == READ) {          // check for READ command

        // ======================================== READ ========================================
        // --------------------------------------------------------------------------------------
        // | Master | S | AD+W |     | RA |     | S | AD+R |            | ACK |      | NACK | P |
        // | Slave  |   |      | ACK |    | ACK |   |      | ACK | DATA |     | DATA |      |   |
        // --------------------------------------------------------------------------------------
        //           <---------- Phase 1 ----------> <---------------- Phase 2 ---------------->

        // Phase 1 - send the Register Address
        if (i2c_nRABytes >= 0) {

            if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SBSEND)) {                        // check if start condition is sent out in master mode

                i2c_master_addressing(I2C0, i2c_slaveAddress, I2C_TRANSMITTER);             // send slave address	with Transmit request

            } else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND)) {                // check if address is sent in master mode

                i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);                       // clear ADDSEND bit

            } else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE)) {                    // check if I2C_DATA is empty (Transmitted Byte Empty)
                if (i2c_nRABytes > 0) {                                                     // check RABytes
                    i2c_data_transmit(I2C0, i2c_regAddress);                                // the master sends the Register Address byte
                }	else {
                    i2c_start_on_bus(I2C0);                                                 // send start condition
                }
                i2c_nRABytes--;
            }

        // Phase 2 - read Data
        } else {

            if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SBSEND)){                          // check if start condition is sent out in master mode

                i2c_master_addressing(I2C0, i2c_slaveAddress, I2C_RECEIVER);                // sends slave address with Receive Request

            }else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND)){                   // check if address is sent in master mode

                if((1 == i2c_nDABytes) || (2 == i2c_nDABytes)){
                    i2c_ack_config(I2C0, I2C_ACK_DISABLE);                                  // clear the ACKEN before the ADDSEND is cleared
                    i2c_interrupt_flag_clear(I2C0,I2C_INT_FLAG_ADDSEND);                    // clear the ADDSEND bit
                }else{
                    i2c_interrupt_flag_clear(I2C0,I2C_INT_FLAG_ADDSEND);                    // clear the ADDSEND bit
                }

            }else if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_RBNE)){                      // check if I2C_DATA is not Empty (Received Byte Not Empty)
                if(i2c_nDABytes > 0){
                        if(3 == i2c_nDABytes){
                            while(!i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_BTC));         // wait until the second last data byte is received into the shift register
                            i2c_ack_config(I2C0, I2C_ACK_DISABLE);                          // send a NACK for the last data byte 
                        }
                        *i2c_rxbuffer++ = i2c_data_receive(I2C0);                           // read a data byte from I2C_DATA
                        i2c_nDABytes--;
                        if(0 == i2c_nDABytes) {
                            i2c_stop_on_bus(I2C0);                                          // send a stop condition
                            i2c_status = 0;                                                 // 0 = Success
                            i2c_ack_config(I2C0, I2C_ACK_ENABLE);
                            i2c_ackpos_config(I2C0, I2C_ACKPOS_CURRENT);
                            i2c_interrupt_disable(I2C0, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);
                        }
                }
                
            }

        }
    }

}



void I2C0_ErrorIRQ_Handler(void)
{
    /* no acknowledge received */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_AERR);
    }

    /* SMBus alert */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SMBALT)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_SMBALT);
    }

    /* bus timeout in SMBus mode */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_SMBTO)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_SMBTO);
    }

    /* over-run or under-run when SCL stretch is disabled */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_OUERR)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_OUERR);
    }

    /* arbitration lost */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_LOSTARB)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_LOSTARB);
    }

    /* bus error */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_BERR)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_BERR);
    }

    /* CRC value doesn't match */
    if(i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_PECERR)){
        i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_PECERR);
    }

    /* disable the error interrupt */
    i2c_interrupt_disable(I2C0,I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);
}


#ifdef AUX45_USE_I2C
/*!
    \brief      handle I2C1 event interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_EventIRQ_Handler(void)
{
    uint16_t k;
    if (i2c_aux_ReadWriteCmd == WRITE) {        // check for WRITE command
        
        // ======================================== WRITE ========================================
        // --------------------------------------------------------------------
        // | Master | S | AD+W |     | RA |     | DATA |     | DATA |     | P |
        // | Slave  |   |      | ACK |    | ACK |      | ACK |      | ACK |   |
        // --------------------------------------------------------------------

        if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SBSEND)) {                            // check if start condition is sent out in master mode

            i2c_master_addressing(I2C1, i2c_aux_slaveAddress, I2C_TRANSMITTER);             // send slave address	with Transmit request

        } else if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND)) {                    // check if address is sent in master mode

            i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_ADDSEND);                           // clear ADDSEND bit

        } else if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_TBE)) {                        // check if I2C_DATA is empty (Transmitted Byte Empty)

            if (i2c_aux_nRABytes > 0) {                                                     // check if the Register Address has been sent
                i2c_data_transmit(I2C1, i2c_aux_regAddress);                                // the master sends the Register Address byte
                i2c_aux_nRABytes--;
            } else {
                if (i2c_aux_nDABytes > 0) {
                    i2c_data_transmit(I2C1, *i2c_aux_txbuffer++);                           // the master sends a data byte
                    i2c_aux_nDABytes--;
                    for(k=0; k<500; k++);
                    if (0 == i2c_aux_nDABytes) {
                        i2c_aux_status = 0;                                                 // 0 = Success
                    }
                    #ifdef __GNUC__
                        asm volatile ("nop");                                               // unoptimizable NOP loop, 500 times to make some clock cycles delay (otherwise DMP writing will fail!! Reason unknown yet.. could be that writing to MPU6050 memory takes a bit more time)
                    #else
                        __asm volatile ("nop");
                    #endif
                } else {
                    i2c_stop_on_bus(I2C1);                                                  // the master sends a stop condition to I2C bus
                    i2c_interrupt_disable(I2C1, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);    // disable the I2C0 interrupt
                }
            }

        }

    } else if (i2c_aux_ReadWriteCmd == READ) {      // check for READ command

        // ======================================== READ ========================================
        // --------------------------------------------------------------------------------------
        // | Master | S | AD+W |     | RA |     | S | AD+R |            | ACK |      | NACK | P |
        // | Slave  |   |      | ACK |    | ACK |   |      | ACK | DATA |     | DATA |      |   |
        // --------------------------------------------------------------------------------------
        //           <---------- Phase 1 ----------> <---------------- Phase 2 ---------------->

        // Phase 1 - send the Register Address
        if (i2c_aux_nRABytes >= 0) {

            if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SBSEND)) {                        // check if start condition is sent out in master mode

                i2c_master_addressing(I2C1, i2c_aux_slaveAddress, I2C_TRANSMITTER);         // send slave address	with Transmit request

            } else if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND)) {                // check if address is sent in master mode

                i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_ADDSEND);                       // clear ADDSEND bit

            } else if (i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_TBE)) {                    // check if I2C_DATA is empty (Transmitted Byte Empty)
                if (i2c_aux_nRABytes > 0) {                                                 // check RABytes
                    i2c_data_transmit(I2C1, i2c_aux_regAddress);                            // the master sends the Register Address byte
                } else {
                    i2c_start_on_bus(I2C1);                                                 // send start condition
                }
                i2c_aux_nRABytes--;
            }

        // Phase 2 - read Data
        } else {

            if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SBSEND)){                          // check if start condition is sent out in master mode        

                i2c_master_addressing(I2C1, i2c_aux_slaveAddress, I2C_RECEIVER);            // sends slave address with Receive Request

            }else if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_ADDSEND)){                   // check if address is sent in master mode

                if((1 == i2c_aux_nDABytes) || (2 == i2c_aux_nDABytes)){
                        i2c_ack_config(I2C1, I2C_ACK_DISABLE);                              // clear the ACKEN before the ADDSEND is cleared            
                        i2c_interrupt_flag_clear(I2C1,I2C_INT_FLAG_ADDSEND);                // clear the ADDSEND bit
                }else{            
                        i2c_interrupt_flag_clear(I2C1,I2C_INT_FLAG_ADDSEND);                // clear the ADDSEND bit
                }

            }else if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_RBNE)){                      // check if I2C_DATA is not Empty (Received Byte Not Empty)				
                if(i2c_aux_nDABytes > 0){
                    if(3 == i2c_aux_nDABytes){
                        while(!i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_BTC));             // wait until the second last data byte is received into the shift register                
                        i2c_ack_config(I2C1, I2C_ACK_DISABLE);                              // send a NACK for the last data byte 
                    }            
                    *i2c_aux_rxbuffer++ = i2c_data_receive(I2C1);                           // read a data byte from I2C_DATA
                    i2c_aux_nDABytes--;
                    if(0 == i2c_aux_nDABytes){
                        i2c_stop_on_bus(I2C1);                                              // send a stop condition
                        i2c_aux_status = 0;                                                 // 0 = Success
                        i2c_ack_config(I2C1, I2C_ACK_ENABLE);
                        i2c_ackpos_config(I2C1, I2C_ACKPOS_CURRENT);
                        i2c_interrupt_disable(I2C1, I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);
                    }
                }
                
            }

        }
    }
}

/*!
    \brief      handle I2C1 error interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_ErrorIRQ_Handler(void)
{
    /* no acknowledge received */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_AERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_AERR);
    }

    /* SMBus alert */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SMBALT)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_SMBALT);
    }

    /* bus timeout in SMBus mode */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_SMBTO)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_SMBTO);
    }

    /* over-run or under-run when SCL stretch is disabled */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_OUERR)){
       i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_OUERR);
    }

    /* arbitration lost */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_LOSTARB)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_LOSTARB);
    }

    /* bus error */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_BERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_BERR);
    }

    /* CRC value doesn't match */
    if(i2c_interrupt_flag_get(I2C1, I2C_INT_FLAG_PECERR)){
        i2c_interrupt_flag_clear(I2C1, I2C_INT_FLAG_PECERR);
    }

    /* disable the error interrupt */
    i2c_interrupt_disable(I2C1,I2C_INT_ERR | I2C_INT_BUF | I2C_INT_EV);
}
#endif

