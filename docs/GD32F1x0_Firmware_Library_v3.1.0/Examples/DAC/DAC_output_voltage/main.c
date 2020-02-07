/*!
    \file  main.c
    \brief DAC0 output voltage
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
void dac_config(void);

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
    dac_config();
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
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_DAC);
}

/*!
    \brief      configure GPIO function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_4);
}

/*!
    \brief      configure DAC function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_config(void)
{
    dac0_trigger_source_config(DAC_TRIGGER_SOFTWARE);
    dac0_output_buffer_disable();
    /* set data left 12b alignment */
    dac0_data_set(DAC_ALIGN_12B_L,0x7FF0);
    /* enable DAC0 */
    dac0_enable();
    dac0_software_trigger_enable();
}
