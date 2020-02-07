/*!
    \file  main.c
    \brief UASRT receiver timeout
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include <stdio.h>
#include "gd32f1x0_eval.h"

uint8_t rxbuffer[64];
uint8_t txbuffer[64];
extern __IO uint8_t txcount; 
extern __IO uint16_t rxcount; 
void nvic_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t i=0, j=0;
    nvic_config();
    gd_eval_com_init(EVAL_COM1);
    printf("a usart receive timeout test example!");
    
    while (1){
        if(0 == rxcount){
            /* enable the USART receive interrupt */
            usart_interrupt_enable(EVAL_COM1, USART_INT_RBNE);
        }else{
            /* enable the USART receive timeout and configure the time of timeout */
            usart_receiver_timeout_enable(EVAL_COM1);
            usart_receiver_timeout_config(EVAL_COM1, 115200*3);

            while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_RT));
            
            for(i=0; i<rxcount; i++){ 
                txbuffer[i]=rxbuffer[j++];
            } 
            /* disable the USART receive interrupt and enable the USART transmit interrupt */
            usart_interrupt_disable(EVAL_COM1, USART_INT_RBNE);
            usart_interrupt_enable(EVAL_COM1, USART_INT_TBE);
            while(txcount < rxcount);
            usart_flag_clear(EVAL_COM1, USART_FLAG_RT);
            txcount=0;
            rxcount=0; 
            i=0;
            j=0;
        }
    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
    return ch;
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_irq_enable(USART0_IRQn, 0, 1);
}
