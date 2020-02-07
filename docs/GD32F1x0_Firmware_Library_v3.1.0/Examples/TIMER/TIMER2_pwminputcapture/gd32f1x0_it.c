/*!
    \file  gd32f1x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_it.h"
#include <stdio.h>

__IO uint32_t ic1value = 0,ic2value = 0;
__IO uint16_t dutycycle = 0;
__IO float frequency = 0;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
}

/**
  * @brief  this function handles TIMER2 interrupt request
  * @param  None
  * @retval None
  */
void TIMER2_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_CH0)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_CH0);
        /* read channel 0 capture value */
        ic1value = timer_channel_capture_value_register_read(TIMER2,TIMER_CH_0)+1;

        if(0 != ic1value){
            /* read channel 1 capture value */
            ic2value = timer_channel_capture_value_register_read(TIMER2,TIMER_CH_1)+1;

            /* calculate the duty cycle value */
            dutycycle = (ic2value * 100) / ic1value;
            /* calculate the frequency value */
            frequency = (float)1000000 / ic1value;

            printf("\r\nthe value1 is %d, the value2 is %d\n", ic1value, ic2value);
            printf("\r\nthe count is %d\n",(ic1value - ic2value));
            printf("\r\nthe dutycycle is %d\n", dutycycle);
            printf("\r\nthe frequence is %f\r\n", frequency);
        }else{
            dutycycle = 0;
            frequency = 0;
        }
    }
}
