/*!
    \file  main.c
    \brief wwdgt delay feed demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include "systick.h"
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
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    gd_eval_led_on(LED1);
    gd_eval_led_on(LED2);

    systick_config();

    /* check if the system has resumed from WWDGT reset */
    if (RESET != rcu_flag_get(RCU_FLAG_WWDGTRST)){   
        /* WWDGTRST flag set */
        gd_eval_led_on(LED1);
        rcu_all_reset_flag_clear();
        while(1);
    }else{
        delay_1ms(150);
        gd_eval_led_off(LED1);
    }

    /* enable WWDGT clock */
    rcu_periph_clock_enable(RCU_WWDGT);
    
    /* enable WWDGT and set counter value to 127, WWDGT timeout = ~455 us * 64 = 29.2 ms.
       in this case the refresh window is: ~455 * (127-80)= 21.4ms < refresh window < ~455 * 64 =29.2ms.
       set window value to 80; WWDGT counter should be refreshed only when the counter
       is below 80 (and greater than 64) otherwise a reset will be generated.
       WWDGT clock counter = (PCLK1 (72MHz)/4096)/8 = 2200Hz (~455 us) */
    wwdgt_config(127,80,WWDGT_CFG_PSC_DIV8);
    wwdgt_enable();

    while (1){
        gd_eval_led_toggle(LED2);
        /* insert 25 ms delay */
        delay_1ms(25);

        /* update WWDGT counter */
        wwdgt_counter_update(127);
    }
}
