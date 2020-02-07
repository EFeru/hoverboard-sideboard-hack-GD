/*!
    \file  main.c
    \brief USART HalfDuplex transmitter and receiver
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
#define TRANSMIT_SIZE0   (ARRAYNUM(transmitter_buffer0) - 1)
#define TRANSMIT_SIZE1   (ARRAYNUM(transmitter_buffer1) - 1)

uint8_t transmitter_buffer0[] = "\n\ra usart half-duplex test example!\n\r";
uint8_t transmitter_buffer1[] = "\n\ra usart half-duplex test example!\n\r";
uint8_t receiver_buffer0[TRANSMIT_SIZE1];
uint8_t receiver_buffer1[TRANSMIT_SIZE0];
uint8_t transfersize0 = TRANSMIT_SIZE0;
uint8_t transfersize1 = TRANSMIT_SIZE1;
__IO uint8_t txcount0 = 0; 
__IO uint16_t rxcount0 = 0; 
__IO uint8_t txcount1 = 0; 
__IO uint16_t rxcount1 = 0; 
ErrStatus state1=ERROR;
ErrStatus state2=ERROR;

ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length) ;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    /* enable USART and GPIOA clock */
    rcu_periph_clock_disable(RCU_DMA);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_USART1);
    
    /* configure the USART0 Tx pin and USART1 Tx pin */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    
    /* USART0 and USART1 baudrate configuration */
    usart_baudrate_set(USART0, 115200);
    usart_baudrate_set(USART1, 115200);   

    /* enable USART0 half duplex mode*/
    usart_halfduplex_enable(USART0);
    /* enable USART1 half duplex mode*/
    usart_halfduplex_enable(USART1);
    
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    /* enable USART */
    usart_enable(USART0);
    usart_enable(USART1);
    
    /* USART0 transmit and USART1 receive*/
    usart_data_receive(USART1);
    while(transfersize0--)
    {
        /* wait until end of transmit */
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
        usart_data_transmit(USART0, transmitter_buffer0[txcount0++]);
 
        while(RESET == usart_flag_get(USART1, USART_FLAG_RBNE));
        /* store the received byte in the receiver_buffer1 */
        receiver_buffer1[rxcount0++] = usart_data_receive(USART1);
    }

    usart_data_receive(USART0);
    /* USART1 transmit and USART0 receive*/
    while(transfersize1--)
    { 
        /* wait until end of transmit */
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
        usart_data_transmit(USART1, transmitter_buffer1[txcount1++]);

        while(RESET == usart_flag_get(USART0, USART_FLAG_RBNE));
        receiver_buffer0[rxcount1++] = usart_data_receive(USART0);
    }
  
    /* compare the received data with the send ones */
    state1 = memory_compare(transmitter_buffer0, receiver_buffer1, TRANSMIT_SIZE0);
    state2 = memory_compare(transmitter_buffer1, receiver_buffer0, TRANSMIT_SIZE1);
    if(SUCCESS==state1){
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
    }else{
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED2); 
    }       
    while (1)
    {
    }
}

/*!
    \brief      memory compare function
    \param[in]  src : source data
    \param[in]  dst : destination data
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t* src, uint8_t* dst, uint16_t length) 
{
    while (length--){
        if (*src++ != *dst++){
            return ERROR;
        }
    }
    return SUCCESS;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
    return ch;
}
