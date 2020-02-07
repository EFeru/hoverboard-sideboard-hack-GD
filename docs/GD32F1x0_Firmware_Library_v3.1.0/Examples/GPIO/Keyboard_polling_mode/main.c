/*!
    \file  main.c
    \brief keyboard polling mode
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
#include "systick.h"
#include "gd32f1x0_eval.h"

#include <stdio.h>

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    systick_config();
    gd_eval_led_init(LED1);
    gd_eval_key_init(KEY_TAMPER,KEY_MODE_GPIO);

    while(1){
        /* check whether the button is pressed */
        if(RESET == gd_eval_key_state_get(KEY_TAMPER)){
            delay_1ms(100);
            /* check whether the button is pressed */
            if(RESET == gd_eval_key_state_get(KEY_TAMPER)){
                gd_eval_led_toggle(LED1);
            }
        }
    }
}
