/*!
    \file  main.c
    \brief ADC_regular_channel_with_DMA
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

#ifdef GD32F130_150
#define BOARD_ADC_CHANNEL ADC_CHANNEL_11
#define ADC_GPIO_PIN GPIO_PIN_1
#elif defined GD32F170_190
#define BOARD_ADC_CHANNEL ADC_CHANNEL_10
#define ADC_GPIO_PIN GPIO_PIN_0
#endif /* GD32F130_150 */

uint16_t adc_value;

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
    /* DMA configuration */
    dma_config();
    /* ADC configuration */
    adc_config();

    while(1);
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
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC);
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
    gpio_mode_set(GPIOC,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC_GPIO_PIN);
}

/*!
    \brief      configure the dma peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_deinit(DMA_CH0);
    dma_periph_address_config(DMA_CH0,(uint32_t)(&ADC_RDATA));
    dma_memory_address_config(DMA_CH0,(uint32_t)(&adc_value));
    dma_transfer_direction_config(DMA_CH0,DMA_PERIPHERAL_TO_MEMORY);
    dma_memory_width_config(DMA_CH0,DMA_MEMORY_WIDTH_16BIT);
    dma_periph_width_config(DMA_CH0,DMA_PERIPHERAL_WIDTH_16BIT);
    dma_priority_config(DMA_CH0,DMA_PRIORITY_HIGH);
    dma_transfer_number_config(DMA_CH0,1);
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
    adc_channel_length_config(ADC_REGULAR_CHANNEL,1);
    /* ADC regular channel config */
    adc_regular_channel_config(0,BOARD_ADC_CHANNEL,ADC_SAMPLETIME_239POINT5);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
    /* ADC external trigger source config */
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_SWRCST);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* enable ADC interface */
    adc_enable();
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
    /* ADC DMA function enable */
    adc_dma_mode_enable();
    /* ADC contineous function enable */
    adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}
