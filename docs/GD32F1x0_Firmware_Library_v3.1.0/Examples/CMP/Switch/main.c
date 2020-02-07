/*!
    \file  main.c
    \brief comparator switch
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
    /* configure RCU */
    rcu_config();
    
    /* configure GPIO */
    gpio_config();
    
    /* configure DAC */
    dac_config();
    
    /* set data for DAC0 */
    dac0_data_set(DAC_ALIGN_12B_R, 0xA00);
    
    dac0_software_trigger_enable();
    
    /* configure CMP0 */
    cmp_mode_init(CMP0, CMP_VERYLOWSPEED, CMP_1_2VREFINT, CMP_HYSTERESIS_NO);
    cmp_output_init(CMP0, CMP_OUTPUT_NONE, CMP_OUTPUT_POLARITY_NOINVERTED);
    
    /* enable CMP0 switch */
    cmp_switch_enable();
    
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
    rcu_periph_clock_enable(RCU_DAC);
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
    /* configure PA4 */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_PULLUP, GPIO_PIN_4);
    /* configure PA6 */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_6);
}

/*!
    \brief      configure dac
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_config(void)
{
    dac0_trigger_source_config(DAC_TRIGGER_SOFTWARE);
    dac0_output_buffer_enable();
    dac0_enable();
}
