/*!
    \file  main.c
    \brief low voltage detector
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

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* NVIC config */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE0_SUB4);
    nvic_irq_enable(LVD_IRQn,0,0);
    /* clock enable */
    rcu_periph_clock_enable(RCU_PMU);
    /* led1 config */
    gd_eval_led_init (LED1);
    /* led1 turn on */
    gd_eval_led_on(LED1);
    /* EXTI_16 config */
    exti_init(EXTI_16, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    /* configure the lvd threshold to 2.9v(gd32f130_150) or 4.5v(gd32f170_190), and enable the lvd */
    pmu_lvd_select(PMU_LVDT_7);
          
    while(1);
}
