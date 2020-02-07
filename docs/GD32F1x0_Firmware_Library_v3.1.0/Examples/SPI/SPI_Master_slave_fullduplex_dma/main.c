/*!
    \file  main.c
    \brief SPI fullduplex communication use DMA 
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

#define arraysize         10

uint8_t spi0_send_array[arraysize] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t spi1_send_array[arraysize] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t spi0_receive_array[arraysize]; 
uint8_t spi1_receive_array[arraysize];
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint8_t length);

void rcu_config(void);
void gpio_config(void);
void dma_config(void);
void spi_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* init led1 and led2 */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    /* peripheral clock enable */
    rcu_config();
    /* GPIO config */
    gpio_config();
    /* DMA config */
    dma_config();
    /* SPI config */
    spi_config();

    /* SPI enable */
    spi_enable(SPI1);
    spi_enable(SPI0);
    
    /* DMA channel enable */
    /* SPI1_Tx DMA channel */
    dma_channel_enable(DMA_CH4);
    /* SPI0_Tx DMA channel */
    dma_channel_enable(DMA_CH2); 
    /* SPI1_Rx DMA channel */
    dma_channel_enable(DMA_CH3);
    /* SPI0_Rx DMA channel */
    dma_channel_enable(DMA_CH1);
    
    /* SPI DMA enable */
    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI1, SPI_DMA_RECEIVE);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI0, SPI_DMA_RECEIVE);

    /* wait dma transmit complete */
    while(!dma_flag_get(DMA_CH2,DMA_INT_FLAG_FTF));
    while(!dma_flag_get(DMA_CH4,DMA_INT_FLAG_FTF));
    while(!dma_flag_get(DMA_CH3,DMA_INT_FLAG_FTF));
    while(!dma_flag_get(DMA_CH1,DMA_INT_FLAG_FTF));

    /* compare receive data with send data */
    if(memory_compare(spi1_receive_array, spi0_send_array, arraysize))
        gd_eval_led_on(LED1);
    else
        gd_eval_led_off(LED1);
    if(memory_compare(spi0_receive_array, spi1_send_array, arraysize))
        gd_eval_led_on(LED2);
    else
        gd_eval_led_off(LED2);

    while (1);
}

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_DMA);
    rcu_periph_clock_enable(RCU_SPI0);
    rcu_periph_clock_enable(RCU_SPI1);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* SPI0 GPIO config: SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    /* SPI1 GPIO config: SCK/PB13, MISO/PB14, MOSI/PB15 */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_13 | GPIO_PIN_14 |GPIO_PIN_15);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13 | GPIO_PIN_14 |GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_14 |GPIO_PIN_15);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_parameter_struct  dma_init_struct;
    /* SPI0 transmit dma config */
    dma_deinit(DMA_CH2);   
    dma_init_struct.periph_addr = (uint32_t )&SPI_DATA(SPI0);
    dma_init_struct.memory_addr = (uint32_t)spi0_send_array;
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.number = arraysize;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA_CH2, dma_init_struct);
    /* SPI1 transmit dma config */
    dma_deinit(DMA_CH4);
    dma_init_struct.periph_addr = (uint32_t )&SPI_DATA(SPI1);
    dma_init_struct.memory_addr = (uint32_t)spi1_send_array;
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH ;
    dma_init(DMA_CH4, dma_init_struct);
    /* SPI0 receive dma config */
    dma_deinit(DMA_CH1);
    dma_init_struct.periph_addr = (uint32_t )&SPI_DATA(SPI0);
    dma_init_struct.memory_addr = (uint32_t)spi0_receive_array;
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority = DMA_PRIORITY_LOW;
    dma_init(DMA_CH1, dma_init_struct);
    /* SPI1 receive dma config */
    dma_deinit(DMA_CH3);
    dma_init_struct.periph_addr = (uint32_t )&SPI_DATA(SPI1);
    dma_init_struct.memory_addr = (uint32_t)spi1_receive_array;
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
    dma_init(DMA_CH3, dma_init_struct);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* SPI1 parameter config */
    spi_init_struct.device_mode = SPI_SLAVE;
    spi_init_struct.nss         = SPI_NSS_SOFT;
    spi_init(SPI1, &spi_init_struct);
    
}

/*!
    \brief      memory compare function
    \param[in]  src : source data pointer
    \param[in]  dst : destination data pointer
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint8_t length) 
{
    while (length--){
        if (*src++ != *dst++)
            return ERROR;
    }
    return SUCCESS;
}
