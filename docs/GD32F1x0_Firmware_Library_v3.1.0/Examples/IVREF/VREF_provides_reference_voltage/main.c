/*!
    \file  main.c
    \brief VREF provides reference voltage
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include "gd32f1x0_libopt.h"
#include <stdio.h>

void rcu_config(void);
void gpio_config(void);
void vref_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    rcu_config();
    gpio_config();
    vref_config();
    while(1);
}

/*!
    \brief      configure RCU function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* GPIOB clock enable */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* IVREF clock enable */
    rcu_periph_clock_enable(RCU_OPAIVREF);
}

/*!
    \brief      configure VREF function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void vref_config(void)
{
    ivref_deinit();
    vref_mode_set(VREF_DISCONNECT_EXTERNAL_CAP);
    vref_precision_trim_value_set(VREF_VOLT_PRECISION_TRIM_15);
    vref_enable();
}

/*!
    \brief      configure GPIO function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    gpio_mode_set(GPIOB,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_1);
}
