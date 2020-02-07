/*!
    \file  main.c
    \brief TIMER0 6-steps demo
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

void gpio_config(void);
void timer_config(void);
void nvic_config(void);

/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */
void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    /*configure PA8 PA9 PA10(TIMER0 CH0 CH1 CH2) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_8);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_10);

    /*configure PB13 PB14 PB15(TIMER0 CH0N CH1N CH2N) as alternate function*/
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);

    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_13);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_14);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_15);

    /*configure PA6(TIMER0 BKIN) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_6);
}

/**
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
  */
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn, 0, 1);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
    /* -----------------------------------------------------------------------
    TIMER0 configuration:
    generate 3 complementary PWM signal.
    TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 72 
    so the TIMER0 counter clock used is 1MHz.
    insert a dead time equal to 72/systemcoreclock =1us 
    configure the break feature, active at low level, and using the automatic
    output enable feature.
    use the locking parameters level0.
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    timer_break_parameter_struct timer_breakpara;

    rcu_periph_clock_enable(RCU_TIMER0);

    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 599;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* CH0/CH0N,CH1/CH1N and CH2/CH2N configuration in timing mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_HIGH;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;

    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_2, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0,299);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_TIMING);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0 ,TIMER_OC_SHADOW_ENABLE);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1,299);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_TIMING);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2,299);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_TIMING);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);

    /* automatic output enable, break, dead time and lock configuration*/
    timer_breakpara.runoffstate     = TIMER_ROS_STATE_ENABLE;
    timer_breakpara.ideloffstate    = TIMER_IOS_STATE_ENABLE ;
    timer_breakpara.deadtime        = 72;
    timer_breakpara.breakpolarity   = TIMER_BREAK_POLARITY_LOW;
    timer_breakpara.outputautostate = TIMER_OUTAUTO_ENABLE;
    timer_breakpara.protectmode     = TIMER_CCHP_PROT_OFF;
    timer_breakpara.breakstate      = TIMER_BREAK_ENABLE;
    timer_break_config(TIMER0, &timer_breakpara);

    /* TIMER0 primary output function enable */
    timer_primary_output_config(TIMER0, ENABLE);

    /* TIMER0 channel control update interrupt enable */
    timer_interrupt_enable(TIMER0, TIMER_INT_CMT);
    /* TIMER0 break interrupt disable */
    timer_interrupt_disable(TIMER0, TIMER_INT_BRK);

    /* TIMER0 counter enable */
    timer_enable(TIMER0);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gpio_config(); 
    systick_config();
    nvic_config();
    timer_config();

    while (1);
}
