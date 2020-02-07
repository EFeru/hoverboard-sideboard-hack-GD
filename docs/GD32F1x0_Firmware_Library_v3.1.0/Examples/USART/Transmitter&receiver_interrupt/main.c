/*!
    \file  main.c
    \brief USART transmit and receive interrupt
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

#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE   (ARRAYNUM(transmitter_buffer) - 1)

uint8_t transmitter_buffer[] = "\n\rUSART interrupt test\n\r";
uint8_t receiver_buffer[32];
uint8_t transfersize = TRANSMIT_SIZE;
uint8_t receivesize = 32;
__IO uint8_t txcount = 0; 
__IO uint16_t rxcount = 0; 


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
     
    gd_eval_com_init(EVAL_COM1);
    
    /* enable USART TBE interrupt */  
    usart_interrupt_enable(EVAL_COM1, USART_INT_TBE);
    
    /* wait until USART send the transmitter_buffer */
    while(txcount < transfersize);
    
    while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC));
    
    usart_interrupt_enable(EVAL_COM1, USART_INT_RBNE);
    
    /* wait until USART receive the receiver_buffer */
    while(rxcount < receivesize);
    if(rxcount == receivesize)
        printf("\n\rUSART receive successfully!\n\r");
      
    while (1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
    return ch;
}
