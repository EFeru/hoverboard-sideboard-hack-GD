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

extern ErrStatus test_flag_interrupt;
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

/*!
    \brief      this function handles CAN0 RX0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN0_RX0_IRQHandler(void)
{
    can_receive_message_struct receive_message;
    /* initialize receive message */
    receive_message.rx_sfid=0x00;
    receive_message.rx_efid=0x00;
    receive_message.rx_ff=0;
    receive_message.rx_dlen=0;
    receive_message.rx_fi=0;
    receive_message.rx_data[0]=0x00;
    receive_message.rx_data[1]=0x00;
    
    /* check the receive message */
    can_message_receive(CAN0, CAN_FIFO0, &receive_message);
    
    if((0x1234 == receive_message.rx_efid) && (CAN_FF_EXTENDED == receive_message.rx_ff)
        && (2 == receive_message.rx_dlen) && (0xCADE == (receive_message.rx_data[1]<<8|receive_message.rx_data[0]))){
        test_flag_interrupt = SUCCESS; 
    }else{
        test_flag_interrupt = ERROR; 
    }
}
/*!
    \brief      this function handles CAN1 RX0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN1_RX0_IRQHandler(void)
{
    can_receive_message_struct receive_message;
    /* initialize receive message */
    receive_message.rx_sfid=0x00;
    receive_message.rx_efid=0x00;
    receive_message.rx_ff=0;
    receive_message.rx_dlen=0;
    receive_message.rx_fi=0;
    receive_message.rx_data[0]=0x00;
    receive_message.rx_data[1]=0x00;

    /* check the receive message */
    can_message_receive(CAN1, CAN_FIFO0, &receive_message);

    if((0x1234 == receive_message.rx_efid) && (CAN_FF_EXTENDED == receive_message.rx_ff)
        && (2 == receive_message.rx_dlen) && (0xCADE == (receive_message.rx_data[1]<<8|receive_message.rx_data[0]))){
        test_flag_interrupt = SUCCESS; 
    }else{
        test_flag_interrupt = ERROR; 
    }
}
