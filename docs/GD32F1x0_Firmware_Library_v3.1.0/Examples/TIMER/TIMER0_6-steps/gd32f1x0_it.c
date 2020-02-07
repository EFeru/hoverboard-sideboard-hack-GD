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
#include "systick.h"

__IO uint32_t step = 1;

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
    timer_event_software_generate(TIMER0,TIMER_EVENT_SRC_CMTG);
}

/*!
    \brief      this function handles TIMER0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER0_BRK_UP_TRG_COM_IRQHandler(void)
{
    timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_CMT);

    switch(step){
    /* next step: step 2 configuration .A-C` breakover---------------------------- */
    case 1: 
        /*  channel0 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCX_ENABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCXN_DISABLE);

        /*  channel1 configuration */
        timer_channel_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCXN_DISABLE);

        /*  channel2 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCXN_ENABLE);

        step++;
        break; 

    /* next step: step 3 configuration .B-C` breakover---------------------------- */
    case 2:
        /*  channel0 configuration */
        timer_channel_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCXN_DISABLE);

        /*  channel1 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCX_ENABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCXN_DISABLE);

        /*  channel2 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCXN_ENABLE);

        step++;
        break;

    /* next step: step 4 configuration .B-A` breakover---------------------------- */
    case 3:
        /*  channel0 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCXN_ENABLE);

        /*  channel1 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCX_ENABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCXN_DISABLE);

        /*  channel2 configuration */
        timer_channel_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCXN_DISABLE);

        step++;
        break;

    /* next step: step 5 configuration .C-A` breakover---------------------------- */
    case 4:
        /*  channel0 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCXN_ENABLE);

        /*  channel1 configuration */       
        timer_channel_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCXN_DISABLE);

        /*  channel2 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCX_ENABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCXN_DISABLE);

        step++;
        break;

    /* next step: step 6 configuration .C-B` breakover---------------------------- */
    case 5:
        /*  channel0 configuration */
        timer_channel_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCXN_DISABLE);

        /*  channel1 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCXN_ENABLE);

        /*  channel2 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCX_ENABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCXN_DISABLE);

        step++;
        break;
  
    /* next step: step 1 configuration .A-B` breakover---------------------------- */        
    case 6:
        /*  channel0 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCX_ENABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_0,TIMER_CCXN_DISABLE);

        /*  channel1 configuration */
        timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
        timer_channel_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_1,TIMER_CCXN_ENABLE);

        /*  channel2 configuration */
        timer_channel_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCX_DISABLE);
        timer_channel_complementary_output_state_config(TIMER0,TIMER_CH_2,TIMER_CCXN_DISABLE);

        step = 1;
        break;
    }
}
