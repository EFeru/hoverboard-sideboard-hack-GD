/*!
    \file  main.c
    \brief OPA amplify
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
#include "gd32f1x0_eval.h"
#include "systick.h"
#include <stdio.h>

void rcu_config(void);
void gpio_config(void);
void opa_config(void);
void adc_config(void);

uint16_t data;
float value;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    rcu_config();
    systick_config();
    gpio_config();
    opa_config();
    adc_config();
    gd_eval_com_init(EVAL_COM2);
    while(1){
        delay_1ms(2000);
        data = adc_regular_data_read();
        value = (float)data * 5.0 / 4096.0;
        printf("\n\rOPA_Out_Value %5.3fV\n\r",value);
        printf("\n\r");
        printf("\n\r");
    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM2, (uint8_t)ch);
    while (RESET == usart_flag_get(EVAL_COM2, USART_FLAG_TC));
    return ch;
}

/*!
    \brief      configure RCU function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* ADCCLK = APB2/8 */
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV8);
    /* OPAMP clock enable */
    rcu_periph_clock_enable(RCU_OPAIVREF);
    /* GPIOA clock enable */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* GPIOC clock enable */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* ADC clock enable */
    rcu_periph_clock_enable(RCU_ADC);
}

/*!
    \brief      Configure GPIO function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    gpio_mode_set(GPIOC,GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

/*!
    \brief      Configure OPA function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void opa_config(void)
{
    opa_deinit();
    opa_switch_enable(OPA_S1OPA2);
    opa_switch_enable(OPA_S2OPA2);
    opa_enable(OPA2);
}

/*!
    \brief      Configure ADC function
    \param[in]  none
    \param[out] none
    \retval     none
*/ 
void adc_config(void)
{
    /* configure ADC */
    adc_special_function_config(ADC_SCAN_MODE,ENABLE);
    adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);
    adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC_EXTTRIG_REGULAR_SWRCST);
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC_REGULAR_CHANNEL,1);
    adc_regular_channel_config(0,ADC_CHANNEL_13,ADC_SAMPLETIME_41POINT5);
    adc_enable();
    adc_calibration_enable();
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}
