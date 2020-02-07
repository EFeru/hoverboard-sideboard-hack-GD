/*!
    \file  main.c
    \brief comparator output timer input capture
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

void rcu_config(void);
void gpio_config(void);
void timer_config(void);
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
    
    /* configure leds */
    gd_eval_led_init(LED3);
    
    /* configure comparator channel0 */
    cmp_mode_init(CMP0, CMP_VERYLOWSPEED, CMP_1_4VREFINT, CMP_HYSTERESIS_NO);
    cmp_output_init(CMP0, CMP_OUTPUT_TIMER1IC3, CMP_OUTPUT_POLARITY_NOINVERTED);
    
    /* configure TIMER */
    timer_config();
    
    /* enable CMP0 */
    cmp_enable(CMP0);
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
    rcu_periph_clock_enable(RCU_GPIOC);
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
    /* configure PB11 */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11) ;
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_11);

    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1) ;
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
    timer_init_parameter.prescaler = 0;
    timer_init_parameter.counterdirection = TIMER_COUNTER_UP;
    timer_init_parameter.period = 65535;
    timer_init_parameter.clockdivision = TIMER_CKDIV_DIV1;
    
    timer_init(TIMER1, &timer_init_parameter);
    
    /* clear flag */
    timer_flag_clear(TIMER1, TIMER_FLAG_UP);
    
    nvic_irq_enable(TIMER1_IRQn, 0, 0);
    
    /* reset TIMER1 interrupt flag register */
    TIMER_INTF(TIMER1) = 0;
    
    timer_interrupt_enable(TIMER1, TIMER_INT_CH3);
    
    /* enable TIMER1 counter */
    timer_enable(TIMER1);
}
