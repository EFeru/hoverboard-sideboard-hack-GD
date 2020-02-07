/*!
    \file  main.c
    \brief deepsleep wakeup through exti interrupt
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include "gd32f1x0_eval.h"
#include "systick.h"
#include "main.h"

void led_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{	  
    /* systick config */
    systick_config();
    /* led config */
    led_config();
    /* clock enable */
    rcu_periph_clock_enable(RCU_PMU);
    /* wakeup key config */
    gd_eval_key_init(KEY_WAKEUP,KEY_MODE_GPIO);
    /* tamper key EXTI config */
    gd_eval_key_init(KEY_TAMPER,KEY_MODE_EXTI);
    /* press wakeup key to enter deepsleep mode and use tamper key generate a exti interrupt to wakeup mcu */
    while(1){
        if(RESET == gpio_input_bit_get(WAKEUP_KEY_GPIO_PORT,WAKEUP_KEY_PIN))
            pmu_to_deepsleepmode(PMU_LDO_LOWPOWER,WFI_CMD);
    }
}

/*!
    \brief      toggle the led
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0;

    if (timingdelaylocal != 0x00){

        if(timingdelaylocal < 200){
            gd_eval_led_on(LED1);
            gd_eval_led_on(LED2);
            gd_eval_led_on(LED3);
            gd_eval_led_on(LED4);
        }else{
            gd_eval_led_off(LED1);
            gd_eval_led_off(LED2);
            gd_eval_led_off(LED3);
            gd_eval_led_off(LED4);
        }
        timingdelaylocal--;
    }else{
        timingdelaylocal = 400;
    }
}

/*!
    \brief      led config
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init (LED1);
    gd_eval_led_init (LED2);
    gd_eval_led_init (LED3);
    gd_eval_led_init (LED4);
}
