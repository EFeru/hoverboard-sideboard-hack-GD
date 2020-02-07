/*!
    \file  main.c
    \brief IREF provides reference current
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
void iref_config(void);

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
    iref_config();
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
    \brief      configure IREF function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void iref_config(void)
{
    ivref_deinit();
    iref_mode_set(IREF_MODE_HIGH_CURRENT);
    iref_enable();
    /* set iref sink mode */
    iref_sink_set(IREF_SOURCE_CURRENT);
    iref_precision_trim_value_set(IREF_CUR_PRECISION_TRIM_16);
    iref_step_data_config(IREF_CUR_STEP_DATA_23);
}

/*!
    \brief      configure GPIO function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    gpio_mode_set(GPIOB,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0);
}
