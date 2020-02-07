/*!
    \file  main.c
    \brief TIMER trigger injected channel of ADC
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
#include "systick.h"

uint16_t adc_value[16];

void rcu_config(void);
void gpio_config(void);
void nvic_config(void);
void timer_config(void);
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
    /* TIMER configuration */
    timer_config();
    /* ADC configuration */
    adc_config();

    timer_enable(TIMER1);
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
    /* enable timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
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
    /* configures led GPIO */
    gd_eval_led_init(LED2);

    /* config the GPIO as analog mode */
    gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
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
    \brief      configure the timer peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 7199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode1 */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;

    timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,3999);
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);
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
    adc_channel_length_config(ADC_INSERTED_CHANNEL,4);

    /* ADC inserted channel config */
    adc_inserted_channel_config(0,ADC_CHANNEL_0,ADC_SAMPLETIME_239POINT5);
    adc_inserted_channel_config(1,ADC_CHANNEL_1,ADC_SAMPLETIME_239POINT5);
    adc_inserted_channel_config(2,ADC_CHANNEL_2,ADC_SAMPLETIME_239POINT5);
    adc_inserted_channel_config(3,ADC_CHANNEL_3,ADC_SAMPLETIME_239POINT5);

    /* ADC external trigger enable */
    adc_external_trigger_config(ADC_INSERTED_CHANNEL,ENABLE);
    /* ADC external trigger source config */
    adc_external_trigger_source_config(ADC_INSERTED_CHANNEL,ADC_EXTTRIG_INSERTED_T1_CH0);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* enable ADC interface */
    adc_enable();
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
    /* ADC SCAN function enable */
    adc_special_function_config(ADC_SCAN_MODE,ENABLE);
}
