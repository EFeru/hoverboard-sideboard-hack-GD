/*!
    \file  gd32f1x0_it.h
    \brief the header file of the ISR
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
#include "i2c_it.h"
#include "config.h"
#include "util.h"

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
    while (1){
    }
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
    while (1){
    }
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
    while (1){
    }
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
    while (1){
    }
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
    tick_count_increment();
    delay_decrement();
}

/*!
    \brief      this function handles the USART0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {    // Check for IDLE line interrupt
        usart_flag_clear(USART0, USART_FLAG_IDLE);                          // Clear IDLE line flag (otherwise it will continue to enter interrupt)
        usart0_rx_check();                                                  // Check for data to process
    }
}

/*!
    \brief      this function handles the USART1 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) {    // Check for IDLE line interrupt
        usart_flag_clear(USART1, USART_FLAG_IDLE);                          // Clear IDLE line flag (otherwise it will continue to enter interrupt)
        usart1_rx_check();                                                  // Check for data to process
    }
}

/*!
    \brief      this function handles I2C0 event interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C0_EV_IRQHandler(void)
{
    I2C0_EventIRQ_Handler();
}

/*!
    \brief      this function handles I2C0 error interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C0_ER_IRQHandler(void)
{
    I2C0_ErrorIRQ_Handler();
}

#ifdef AUX45_USE_I2C
/*!
    \brief      this function handles I2C1 event interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_EV_IRQHandler(void)
{
    I2C1_EventIRQ_Handler();
}

/*!
    \brief      this function handles I2C1 error interrupt request exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C1_ER_IRQHandler(void)
{
    I2C1_ErrorIRQ_Handler();
}
#endif
