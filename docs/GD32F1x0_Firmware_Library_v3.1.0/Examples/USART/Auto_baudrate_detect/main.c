/*!
    \file  main.c
    \brief Auto baudrate detection
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
#include <stdio.h>

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{ 
    gd_eval_led_init(LED1);
    
    /* USART configuration */
    gd_eval_com_init(EVAL_COM1);
    
    printf("\n\rUSART auto baudrate detection example\n\r");
    /* configure the auto-baud rate method */
    usart_autobaud_detection_mode_config(EVAL_COM1, USART_ABDM_FTOF);
    
    /* enable autobaudrate feature */
    usart_autobaud_detection_enable(EVAL_COM1);
    
    /* wait until receive enable acknowledge flag is set */
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_REA));
    
    /* wait until transmit enable acknowledge flag is set */  
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TEA)); 
    
    /* loop until the end of autobaudrate phase */
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_ABD));
    
    /* if autobaudbate error occurred */
    if(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_ABDE)){
        /* wait until RBNE flag is set */
        while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_RBNE));
        
        /* wait until TBE flag is set */    
        while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
        
        /* USART auto baud rate detection finshed successfully */
        gd_eval_led_on(LED1);
            
        /* check the transfer complete flag */
        while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC));
        
        usart_transmit_config(EVAL_COM1, USART_TRANSMIT_DISABLE);
        usart_receive_config(EVAL_COM1, USART_RECEIVE_DISABLE);
    }
    /* USART disable */
    usart_disable(EVAL_COM1);
    while(1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
    return ch;
}
