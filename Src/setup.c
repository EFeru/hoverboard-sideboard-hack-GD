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
#include "gd32f1x0.h"
#include "systick.h"
#include "setup.h"
#include "defines.h"
#include "config.h"
#include "util.h"

// Global variables


// Private variables
static rcu_periph_enum USART_CLK[USARTn]        = { USART0_CLK,
                                                    USART1_CLK
                                                  };

static uint32_t USART_TX_PIN[USARTn]            = { USART0_TX_PIN,
                                                    USART1_TX_PIN
                                                  };

static uint32_t USART_RX_PIN[USARTn]            = { USART0_RX_PIN,
                                                    USART1_RX_PIN
                                                  };

static dma_channel_enum USART_TX_DMA_CH[USARTn] = { USART0_TX_DMA_CH,
                                                    USART1_TX_DMA_CH
                                                  };

static dma_channel_enum USART_RX_DMA_CH[USARTn] = { USART0_RX_DMA_CH,
                                                    USART1_RX_DMA_CH
                                                  };

static uint32_t USART_TDATA_ADDRESS[USARTn]     = { USART0_TDATA_ADDRESS,
                                                    USART1_TDATA_ADDRESS
                                                  };

static uint32_t USART_RDATA_ADDRESS[USARTn]     = { USART0_RDATA_ADDRESS,
                                                    USART1_RDATA_ADDRESS
                                                  };


void gpio_config(void) {

    /* =========================== Configure LEDs GPIOs =========================== */
    /* enable the GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    /* configure GPIO port */ 
    gpio_mode_set(LED1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_Pin);
    gpio_mode_set(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_Pin);
    gpio_mode_set(LED3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_Pin);
    gpio_mode_set(LED4_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED4_Pin);
    gpio_mode_set(LED5_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED5_Pin);
    gpio_output_options_set(LED1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_Pin);
    gpio_output_options_set(LED2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_Pin);
    gpio_output_options_set(LED3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED3_Pin);
    gpio_output_options_set(LED4_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED4_Pin);
    gpio_output_options_set(LED5_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED5_Pin);

    /* reset GPIO pin */
    gpio_bit_reset(LED1_GPIO_Port, LED1_Pin);
    gpio_bit_reset(LED2_GPIO_Port, LED2_Pin);
    gpio_bit_reset(LED3_GPIO_Port, LED3_Pin);
    gpio_bit_reset(LED4_GPIO_Port, LED4_Pin);
    gpio_bit_reset(LED5_GPIO_Port, LED5_Pin);


    /* =========================== Configure Sensors GPIOs =========================== */
    /* enable the GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    /* configure GPIO port */
    gpio_mode_set(SENSOR1_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SENSOR1_Pin);
    gpio_mode_set(SENSOR2_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SENSOR2_Pin);


    /* =========================== Configure I2C GPIOs =========================== */
    /* enable I2C clock */
    rcu_periph_clock_enable(RCU_GPIOB);
      rcu_periph_clock_enable(MPU_RCU_I2C);

    /* connect PB6 to I2C_SCL and PB7 to I2C_SDA */
    gpio_af_set(MPU_SCL_GPIO_Port, GPIO_AF_1, MPU_SCL_Pin);
    gpio_af_set(MPU_SDA_GPIO_Port, GPIO_AF_1, MPU_SDA_Pin);

    /* configure GPIO port */
    gpio_mode_set(MPU_SCL_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, MPU_SCL_Pin);
    gpio_output_options_set(MPU_SCL_GPIO_Port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, MPU_SCL_Pin);	
    gpio_mode_set(MPU_SDA_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, MPU_SDA_Pin);
    gpio_output_options_set(MPU_SDA_GPIO_Port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, MPU_SDA_Pin);

    #ifdef AUX45_USE_I2C
        /* enable I2C clock */
        rcu_periph_clock_enable(RCU_GPIOA);
        rcu_periph_clock_enable(AUX_RCU_I2C);

        /* connect PB6 to I2C_SCL and PB7 to I2C_SDA */
        gpio_af_set(AUX_SCL_GPIO_Port, GPIO_AF_1, AUX_SCL_Pin);
        gpio_af_set(AUX_SDA_GPIO_Port, GPIO_AF_1, AUX_SDA_Pin);
        
        /* configure GPIO port */
        gpio_mode_set(AUX_SCL_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, AUX_SCL_Pin);
        gpio_output_options_set(AUX_SCL_GPIO_Port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, AUX_SCL_Pin);	
        gpio_mode_set(AUX_SDA_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, AUX_SDA_Pin);
        gpio_output_options_set(AUX_SDA_GPIO_Port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, AUX_SDA_Pin);	
    #endif


    /* =========================== Configure AUX GPIOs =========================== */
    /* configure AUX GPIO port */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

    /* configure GPIO port - inputs */
    gpio_mode_set(AUX1_PU_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AUX1_PU_Pin);

    /* configure GPIO port - outputs */
    gpio_mode_set(AUX2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AUX2_Pin);
    gpio_mode_set(AUX3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AUX3_Pin);
    gpio_output_options_set(AUX2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AUX2_Pin);
    gpio_output_options_set(AUX3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AUX3_Pin);

    /* reset GPIO pin */
    gpio_bit_reset(AUX2_GPIO_Port, AUX2_Pin);
    gpio_bit_reset(AUX3_GPIO_Port, AUX3_Pin);

    #ifdef AUX45_USE_GPIO
        /* configure GPIO port - outputs */
        gpio_mode_set(AUX4_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AUX4_Pin);
        gpio_mode_set(AUX5_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, AUX5_Pin);		
        gpio_output_options_set(AUX4_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AUX4_Pin);
        gpio_output_options_set(AUX5_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AUX5_Pin);
        
            /* reset GPIO pin */
        gpio_bit_reset(AUX4_GPIO_Port, AUX4_Pin);
        gpio_bit_reset(AUX5_GPIO_Port, AUX5_Pin);
    #endif

}


void usart_config(uint32_t selUSART, uint32_t selBaudRate) {

    uint8_t USART_ID = 0U;
    if(selUSART == USART0){
        USART_ID = 0U;
    }
    if(selUSART == USART1){
        USART_ID = 1U;
    }

    /* enable GPIO clock */
    rcu_periph_clock_enable(USART_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(USART_CLK[USART_ID]);

    /* connect port to USARTx_Tx */
    gpio_af_set(USART_GPIO_PORT, USART_AF, USART_TX_PIN[USART_ID]);

    /* connect port to USARTx_Rx */
    gpio_af_set(USART_GPIO_PORT, USART_AF, USART_RX_PIN[USART_ID]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX_PIN[USART_ID]);
    gpio_output_options_set(USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, USART_TX_PIN[USART_ID]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX_PIN[USART_ID]);
    gpio_output_options_set(USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, USART_RX_PIN[USART_ID]);

    /* USART configure */
    usart_deinit(selUSART);
    usart_baudrate_set(selUSART, selBaudRate);
    usart_transmit_config(selUSART, USART_TRANSMIT_ENABLE);
    usart_receive_config(selUSART, USART_RECEIVE_ENABLE);
    usart_oversample_config(selUSART, USART_OVSMOD_16);     // oversampling: {USART_OVSMOD_8, USART_OVSMOD_16}
    usart_sample_bit_config(selUSART, USART_OSB_3BIT);      // sample bit:   {USART_OSB_1BIT, USART_OSB_3BIT }
    usart_enable(selUSART);

    /* enable the USART IDLE line detection interrupt */
    usart_interrupt_enable(selUSART, USART_INT_IDLE);

}


// DMA_CH1 = USART0_TX
// DMA_CH2 = USART0_RX
// DMA_CH3 = USART1_TX
// DMA_CH4 = USART1_RX

void usart_Tx_DMA_config(uint32_t selUSART, uint8_t *pData, uint32_t dSize) {

    dma_parameter_struct dma_init_struct;

    // --------------------------- TX Channel ---------------------------

    uint8_t USART_ID = 0U;
    if(selUSART == USART0){
        USART_ID = 0U;
    }
    if(selUSART == USART1){
        USART_ID = 1U;
    }

    /* enable DMA clock */
      rcu_periph_clock_enable(RCU_DMA);
    
    /* deinitialize DMA channel2 */
    dma_deinit(USART_TX_DMA_CH[USART_ID]);
    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr         = (uint32_t)pData;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width        = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number              = dSize;
    dma_init_struct.periph_addr         = USART_TDATA_ADDRESS[USART_ID];
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width        = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;          // Priorities: *_LOW, *_MEDIUM, *_HIGH, *_ULTRA_HIGH,
    dma_init(USART_TX_DMA_CH[USART_ID], dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(USART_TX_DMA_CH[USART_ID]);
    dma_memory_to_memory_disable(USART_TX_DMA_CH[USART_ID]);

    /* USART DMA enable for transmission */
    usart_dma_transmit_config(selUSART, USART_DENT_ENABLE);

    /* enable DMA channel1 */
    dma_channel_enable(USART_TX_DMA_CH[USART_ID]);

    /* wait DMA channel transfer complete */
    // while (RESET == dma_flag_get(USART_TX_DMA[USART_ID], DMA_FLAG_FTF));

}


void usart_Rx_DMA_config(uint32_t selUSART, uint8_t *pData, uint32_t dSize) {

    dma_parameter_struct dma_init_struct;

    // --------------------------- RX Channel ---------------------------

    uint8_t USART_ID = 0U;
    if(selUSART == USART0){
        USART_ID = 0U;
    }
    if(selUSART == USART1){
        USART_ID = 1U;
    }

    /* enable DMA clock */
      rcu_periph_clock_enable(RCU_DMA);

    /* deinitialize DMA channel4 */
    dma_deinit(USART_RX_DMA_CH[USART_ID]);
    dma_init_struct.direction           = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr         = (uint32_t)pData;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width        = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number              = dSize;
    dma_init_struct.periph_addr         = USART_RDATA_ADDRESS[USART_ID];
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width        = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;          // Priorities: *_LOW, *_MEDIUM, *_HIGH, *_ULTRA_HIGH,
    dma_init(USART_RX_DMA_CH[USART_ID], dma_init_struct);

    /* configure DMA mode */
    dma_circulation_enable(USART_RX_DMA_CH[USART_ID]); 	// dma_circulation_disable(USART_RX_DMA[USART_ID]);
    dma_memory_to_memory_disable(USART_RX_DMA_CH[USART_ID]);

    /* USART DMA enable for reception */	
    usart_dma_receive_config(selUSART, USART_DENR_ENABLE);

    /* enable DMA channel */
    dma_channel_enable(USART_RX_DMA_CH[USART_ID]);

    /* wait DMA channel transfer complete */
    // while (RESET == dma_flag_get(USART_RX_DMA[USART_ID], DMA_FLAG_FTF));	

}


void i2c_config(void) {

    /* I2C clock configure */
    //i2c_clock_config(MPU_I2C, MPU_I2C_SPEED, I2C_DTCY_2);             // I2C duty cycle in fast mode
    i2c_clock_config(MPU_I2C, MPU_I2C_SPEED, I2C_DTCY_16_9);            // I2C duty cycle in fast mode plus
    /* I2C address configure */
    i2c_mode_addr_config(MPU_I2C, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C_OWN_ADDRESS7);
    /* enable I2C */
    i2c_enable(MPU_I2C);
    /* enable acknowledge */
    i2c_ack_config(MPU_I2C, I2C_ACK_ENABLE);
    
    #ifdef AUX45_USE_I2C
        /* I2C clock configure */
        //i2c_clock_config(AUX_I2C, AUX_I2C_SPEED, I2C_DTCY_2);         // I2C duty cycle in fast mode
        i2c_clock_config(AUX_I2C, AUX_I2C_SPEED, I2C_DTCY_16_9);        // I2C duty cycle in fast mode plus
        /* I2C address configure */
        i2c_mode_addr_config(AUX_I2C, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, AUX_I2C_OWN_ADDRESS7);
        /* enable I2C */
        i2c_enable(AUX_I2C);
        /* enable acknowledge */
        i2c_ack_config(AUX_I2C, I2C_ACK_ENABLE);
    #endif

}


void usart_nvic_config(void)
{
    nvic_irq_enable(USART0_IRQn, 0, 1);     // Enable USART0 interrupt
    nvic_irq_enable(USART1_IRQn, 0, 1);     // Enable USART1 interrupt
}


void i2c_nvic_config(void)
{
    /* configure the NVIC peripheral */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);

    nvic_irq_enable(I2C0_EV_IRQn, 0, 3);
    nvic_irq_enable(I2C0_ER_IRQn, 0, 2);

    #ifdef AUX45_USE_I2C
    nvic_irq_enable(I2C1_EV_IRQn, 0, 4);
    nvic_irq_enable(I2C1_ER_IRQn, 0, 1);
    #endif
}


