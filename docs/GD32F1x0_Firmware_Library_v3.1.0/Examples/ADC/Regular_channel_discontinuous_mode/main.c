/*!
    \file  main.c
    \brief ADC discontinuous mode for regular channel
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include <stdio.h>
#include "systick.h"
#include "gd32f1x0_eval.h"

uint16_t adc_value[16];

void rcu_config(void);
void gpio_config(void);
void dma_config(void);
void adc_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* system clocks configuration */
    rcu_config();
    /* GPIO configuration */
    gpio_config();
    /* SYSTICK configuration */
    systick_config();
    /* DMA configuration */
    dma_config();
    /* ADC configuration */
    adc_config();

    while(1){
        delay_1ms(1000);
        /* ADC software trigger enable */
        adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
}
/*!
    \brief      configure the gpio peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode */
    gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
}

/*!
    \brief      configure the dma peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC DMA_channel configuration */
    dma_deinit(DMA_CH0);
    dma_periph_address_config(DMA_CH0,(uint32_t)(&ADC_RDATA));
    dma_memory_address_config(DMA_CH0,(uint32_t)(adc_value));
    dma_transfer_direction_config(DMA_CH0,DMA_PERIPHERAL_TO_MEMORY);
    dma_memory_width_config(DMA_CH0,DMA_MEMORY_WIDTH_16BIT);
    dma_periph_width_config(DMA_CH0,DMA_PERIPHERAL_WIDTH_16BIT);
    dma_priority_config(DMA_CH0,DMA_PRIORITY_HIGH);
    dma_transfer_number_config(DMA_CH0,16);
    dma_periph_increase_disable(DMA_CH0);
    dma_memory_increase_enable(DMA_CH0);
    dma_circulation_enable(DMA_CH0);
    dma_channel_enable(DMA_CH0);
}

/*!
    \brief      configure the adc peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL,8);

    /* ADC regular channel config */
    adc_regular_channel_config(0,ADC_CHANNEL_0,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(1,ADC_CHANNEL_1,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(2,ADC_CHANNEL_2,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(3,ADC_CHANNEL_3,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(4,ADC_CHANNEL_4,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(5,ADC_CHANNEL_5,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(6,ADC_CHANNEL_6,ADC_SAMPLETIME_239POINT5);
    adc_regular_channel_config(7,ADC_CHANNEL_7,ADC_SAMPLETIME_239POINT5);

    /* ADC external trigger enable */
    adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
    /* ADC external trigger source config */
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_SWRCST);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* ADC discontinuous mode */
    adc_discontinuous_mode_config(ADC_REGULAR_CHANNEL,3);
    /* enable ADC interface */
    adc_enable();
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
    /* ADC DMA function enable */
    adc_dma_mode_enable();

    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}
