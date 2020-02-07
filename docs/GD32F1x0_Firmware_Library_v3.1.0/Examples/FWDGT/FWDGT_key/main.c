/*!
    \file  main.c
    \brief fwdgt key demo
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

void irc40k_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* system clocks configuration */
    irc40k_config();
    systick_config();
  
    gd_eval_led_init(LED2);
    gd_eval_key_init(KEY_TAMPER,KEY_MODE_EXTI);
    delay_1ms(50);

    /* enable write access to FWDGT_PSC and FWDGT_RLD registers.
       FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(625,FWDGT_PSC_DIV64);
    fwdgt_enable();

    /* check if the system has resumed from FWDGT reset */
    if (RESET != rcu_flag_get(RCU_FLAG_FWDGTRST)){
        gd_eval_led_on(LED2);
        rcu_all_reset_flag_clear();
      
        while(1);
    }else{
        gd_eval_led_off(LED2);
    }

    while (1);
}

/*!
    \brief      IRC40K configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void irc40k_config(void)
{
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
    /* wait till IRC40K is ready */
    rcu_osci_stab_wait(RCU_IRC40K);
}
