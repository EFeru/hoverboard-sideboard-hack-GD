/*!
    \file  main.c
    \brief SLCD digit display
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
#include "systick.h"
#include "slcd_seg.h"

void rcu_configuration(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint16_t num = 0;
    /* system clocks configuration */
    rcu_configuration();
    /* systick config: 1ms delay */
    systick_configuration();
    /* slcd interface configuration */
    slcd_seg_configuration();

    while(1){
        slcd_seg_number_display(num++);
        delay_1ms(1000);
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_configuration(void)
{
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* PMU backup domain write enable */
    pmu_backup_write_enable();
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
    /* wait for IRC40K stabilization flags */
    rcu_osci_stab_wait(RCU_IRC40K);
    /* configure the RTC clock source selection */
    rcu_slcd_clock_config(RCU_SLCDSRC_IRC40K);
}
