/*!
    \file  main.c
    \brief PWM output by using comparator output
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

uint16_t period = 65535;

void rcu_config(void);
void gpio_config(void);
void timer_config(void);
void cmp_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure RCU */
    rcu_config();
    
    /* configure GPIO */
    gpio_config();
    
    /* configure TIMER */
    timer_config();
    
    /* configure comparator */
    cmp_config();
    
    while(1);
}

/*!
    \brief      configure RCU
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_TIMER1);
    rcu_periph_clock_enable(RCU_CFGCMP);
}

/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure PB3 as PWM output */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_3);
    
    /* configure PA1 as comparator input */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_PULLUP, GPIO_PIN_1);
    
}

/*!
    \brief      configure TIMER
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    /* initialize TIMER1 */
    timer_parameter_struct timer_init_parameter;
    timer_init_parameter.prescaler = 71;
    timer_init_parameter.counterdirection = TIMER_COUNTER_UP;
    timer_init_parameter.period = 65535;
    timer_init_parameter.clockdivision = TIMER_CKDIV_DIV1;
    
    timer_init(TIMER1, &timer_init_parameter);

    /* PWM1 mode configure channel1 in PWM1 mode */
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM1);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, (period/2) + 1);
    timer_channel_output_state_config(TIMER1,TIMER_CH_1, TIMER_CCX_ENABLE);
    /* select OCREFCLR as source for clearing OC2REF */
    timer_channel_output_clear_config(TIMER1, TIMER_CH_1, TIMER_OC_CLEAR_ENABLE);
    timer_ocpre_clear_source_config(TIMER1, TIMER_OCPRE_CLEAR_SOURCE_CLR);

    /* enable TIMER1 counter */
    timer_enable(TIMER1);
}

/*!
    \brief      configure comparator
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cmp_config(void)
{
    /* configure CMP0 */
    cmp_mode_init(CMP0, CMP_HIGHSPEED, CMP_VREFINT, CMP_HYSTERESIS_NO);
    cmp_output_init(CMP0, CMP_OUTPUT_TIMER1OCPRECLR, CMP_OUTPUT_POLARITY_NOINVERTED);
    
    /* enable CMP0 */
    cmp_enable(CMP0);
}
