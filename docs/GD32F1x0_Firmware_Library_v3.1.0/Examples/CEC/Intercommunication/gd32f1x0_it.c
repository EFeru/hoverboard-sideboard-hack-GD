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
#include "gd32f1x0_eval.h"
#include "systick.h"

/* array1 transmission */
uint8_t senddata1[2] = {0xA5,0x5A};
/* array2 transmission */
uint8_t senddata2[2] = {0x5A,0xA5};
/* receive array buffer  */
uint8_t rcvdata[10];
/* sent data pointer */
uint8_t *transdata;
__IO uint8_t rcvstatus = 0U;
__IO uint8_t send_inc = 0U, rcv_inc = 0U;
uint16_t senderrorcode = 0U;
uint16_t rcverrorcode = 0U;
__IO uint8_t bytenum = 0U;

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
        delay_decrement();
}

/*!
    \brief      this function handles EXTI0_1 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
#ifdef GD32F170_190
void EXTI0_1_IRQHandler(void)
{
    if(RESET != exti_flag_get(WAKEUP_KEY_EXTI_LINE)){
        /* Tamper key press */
        bytenum = 2U;
        /* send CEC address */
        cec_data_send(0x12U);
        /* start transmission */
        transdata = senddata1;
        /* start transmission */
        cec_transmission_start();
        exti_flag_clear(WAKEUP_KEY_EXTI_LINE);
    }
}
#endif /* GD32F170_190 */

/*!
    \brief      this function handles EXTI4_15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI4_15_IRQHandler(void)
{
#ifdef GD32F130_150
    if(RESET != exti_flag_get(USER_KEY_EXTI_LINE)){
        /* Tamper key press */
        bytenum = 2U;
        /* send CEC address */
        cec_data_send(0x12U);
        transdata = senddata1;
        /* start transmission */
        cec_transmission_start();
        exti_flag_clear(USER_KEY_EXTI_LINE);
    }
#endif /* GD32F130_150 */
    
    if(RESET != exti_flag_get(TAMPER_KEY_EXTI_LINE)){
        /* Tamper key press */
        bytenum = 2U;
        /* send CEC address */
        cec_data_send(0x12U);
        /* start transmission */
        transdata = senddata2;
        /* start transmission */
        cec_transmission_start();
        exti_flag_clear(TAMPER_KEY_EXTI_LINE);
    }
}

/*!
    \brief      this function handles CEC interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CEC_IRQHandler(void)
{
    /* check if a reception error occured */
    if(cec_interrupt_flag_get(CEC_INTF_RO | CEC_INTF_BRE | CEC_INTF_BPSE | CEC_INTF_BPLE | CEC_INTF_RAE)){
        rcverrorcode = CEC_INTF;
        cec_interrupt_flag_clear(CEC_INTF_RO | CEC_INTF_BRE | CEC_INTF_BPSE | CEC_INTF_BPLE | CEC_INTF_RAE);
        rcvstatus  = 2U;
    }
    /* receive data */
    if(cec_interrupt_flag_get(CEC_INTF_BR)){
        rcvdata[rcv_inc] = cec_data_receive();
        rcv_inc++;
        cec_interrupt_flag_clear(CEC_INTF_BR);
    }
    /* check if the byte received is the last one of the message */
    if(cec_interrupt_flag_get(CEC_INTF_REND)){
        rcvstatus = 1U;
        rcv_inc = 0U;
        cec_interrupt_flag_clear(CEC_INTF_REND);
    }

    /* check if a transmission error occurred */
    if(cec_interrupt_flag_get(CEC_INTF_ARBF | CEC_INTF_TU | CEC_INTF_TERR | CEC_INTF_TAERR)){
        senderrorcode = CEC_INTF;
        cec_interrupt_flag_clear(CEC_INTF_ARBF | CEC_INTF_TU | CEC_INTF_TERR | CEC_INTF_TAERR);
    }
  
    /* check if end of message bit is set in the data to be transmitted */
    if(cec_interrupt_flag_get(CEC_INTF_TEND)){
        cec_interrupt_flag_clear(CEC_INTF_TEND| CEC_INTF_TBR);
        send_inc = 0U;
    }else if(cec_interrupt_flag_get(CEC_INTF_TBR)){
        /* set EOM bit if the byte to be transmitted is the last one of the senddata */
        if(send_inc++ == (bytenum - 1U)){
            cec_transmission_end();
            cec_data_send(*transdata++);
        }else{
            /* send the byte in the transdata */
            cec_data_send(*transdata++);
        }
        cec_interrupt_flag_clear(CEC_INTF_TBR);
    }
}
