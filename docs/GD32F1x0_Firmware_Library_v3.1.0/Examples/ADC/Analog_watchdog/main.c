/*!
    \file  main.c
    \brief ADC analog watchdog
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
#include "gd32f1x0_eval.h"

#ifdef GD32F130_150
#define BOARD_ADC_CHANNEL ADC_CHANNEL_11
#define ADC_GPIO_PIN GPIO_PIN_1
#elif defined GD32F170_190
#define BOARD_ADC_CHANNEL ADC_CHANNEL_10
#define ADC_GPIO_PIN GPIO_PIN_0
#endif

void rcu_config(void);
void gpio_config(void);
void nvic_config(void);
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
    /* NVIC configuration */
    nvic_config();
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
    /* configure led GPIO */
    gd_eval_led_init(LED2);
    /* config the GPIO as analog mode */
    gpio_mode_set(GPIOC,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC_GPIO_PIN);
}

/*!
    \brief      configure interrupt priority
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(ADC_CMP_IRQn, 0, 0);
}

/*!
    \brief      configure the adc peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC regular channel length config */
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
    /* ADC contineous function enable */
    adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);

    /* ADC analog watchdog threshold config */
    adc_watchdog_threshold_config(0x0400,0x0A00);
    /* ADC analog watchdog single channel config */
    adc_watchdog_single_channel_enable(BOARD_ADC_CHANNEL);
    /* ADC interrupt config */
    adc_interrupt_enable(ADC_INT_WDE);

    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}
