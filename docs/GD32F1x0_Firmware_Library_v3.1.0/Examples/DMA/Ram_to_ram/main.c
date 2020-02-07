/*!
    \file  main.c
    \brief transfer data from RAM to RAM
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h" 
#include "gd32f1x0_eval.h"

#define DATANUM                  16

__IO ErrStatus transferflag1 = ERROR;
__IO ErrStatus transferflag2 = ERROR;
__IO ErrStatus transferflag3 = ERROR;
__IO ErrStatus transferflag4 = ERROR;
uint8_t source_address[DATANUM]= {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                                  0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};
uint8_t destination_address1[DATANUM];
uint8_t destination_address2[DATANUM];
uint8_t destination_address3[DATANUM];
uint8_t destination_address4[DATANUM];

void led_config(void);
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int i = 0;
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
    /* initialize LED */
    led_config();
    /* all LED off */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED4);
    /* initialize DMA channel1 */
    dma_deinit(DMA_CH1);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)destination_address1;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = DATANUM;
    dma_init_struct.periph_addr = (uint32_t)source_address;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH1,dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH1);
    dma_memory_to_memory_enable(DMA_CH1);
        
    /* initialize DMA channel2 */
    dma_deinit(DMA_CH2);
    dma_init_struct.memory_addr = (uint32_t)destination_address2;
    dma_init(DMA_CH2,dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH2);
    dma_memory_to_memory_enable(DMA_CH2);
        
    /* initialize DMA channel3 */
    dma_deinit(DMA_CH3);
    dma_init_struct.memory_addr = (uint32_t)destination_address3;
    dma_init(DMA_CH3,dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH3);
    dma_memory_to_memory_enable(DMA_CH3);
        
    /* initialize DMA channel4 */
    dma_deinit(DMA_CH4);
    dma_init_struct.memory_addr = (uint32_t)destination_address4;
    dma_init(DMA_CH4,dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH4);
    dma_memory_to_memory_enable(DMA_CH4);

    /* enable DMA channel1~channel4 */
    dma_channel_enable(DMA_CH1);
    dma_channel_enable(DMA_CH2);
    dma_channel_enable(DMA_CH3);
    dma_channel_enable(DMA_CH4);

    /* wait for DMA transfer complete */
    for(i = 0; i < 200; i++);
    /* compare the data of source_address with data of destination_address */
    transferflag1 = memory_compare(source_address, destination_address1, DATANUM);
    transferflag2 = memory_compare(source_address, destination_address2, DATANUM);
    transferflag3 = memory_compare(source_address, destination_address3, DATANUM);
    transferflag4 = memory_compare(source_address, destination_address4, DATANUM);

    /* if DMA channel1 transfer success,light LED1 */
    if(SUCCESS == transferflag1){
        gd_eval_led_on(LED1);
    }
    /* if DMA channel2 transfer success,light LED2 */
    if(SUCCESS == transferflag2){
        gd_eval_led_on(LED2);
    }
    /* if DMA channel3 transfer success,light LED3 */
    if(SUCCESS == transferflag3){
        gd_eval_led_on(LED3);
    }
    /* if DMA channel4 transfer success,light LED4 */
    if(SUCCESS == transferflag4){
        gd_eval_led_on(LED4);
    }
    
    while (1);
}

/*!
    \brief      LEDs configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init (LED1);
    gd_eval_led_init (LED2);
    gd_eval_led_init (LED3);
    gd_eval_led_init (LED4);
}

/*!
    \brief      memory compare function
    \param[in]  src : source data
    \param[in]  dst : destination data
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length)
{
    while (length--){
        if (*src++ != *dst++){
            return ERROR;
        }
    }
    return SUCCESS;
}
