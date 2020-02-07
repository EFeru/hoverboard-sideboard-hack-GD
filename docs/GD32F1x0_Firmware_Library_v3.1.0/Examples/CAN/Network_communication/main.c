/*!
    \file  main.c
    \brief CAN networking communication in normal mode
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

/* select can */
//#define CAN0_USED
#define CAN1_USED

#ifdef  CAN0_USED
    #define CANX CAN0
#else 
    #define CANX CAN1
#endif
    
extern FlagStatus receive_flag;
uint8_t transmit_number = 0x0;
extern can_receive_message_struct receive_message;
can_trasnmit_message_struct transmit_message;
    
void nvic_config(void);
void led_config(void);
void gpio_config(void);
ErrStatus can_networking(void);
void can_networking_init(can_parameter_struct can_parameter, can_filter_parameter_struct can_filter);
void delay(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    can_parameter_struct            can_init_parameter;
    can_filter_parameter_struct     can_filter_parameter;

    receive_flag = RESET;
    /* configure Tamper key */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_GPIO);
    /* configure GPIO */
    gpio_config();
    /* configure USART */
    gd_eval_com_init(EVAL_COM2);
    /* configure NVIC */
    nvic_config();
    /* configure leds */
    led_config();
    /* set all leds off */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    /* initialize CAN */
    can_networking_init(can_init_parameter, can_filter_parameter);
    /* enable phy */
#ifdef CAN0_USED 
    can_phy_enable(CANX);
#endif
    /* enable CAN receive FIFO0 not empty interrupt */
    can_interrupt_enable(CANX, CAN_INTEN_RFNEIE0);
    
    /* initialize transmit message */
    transmit_message.tx_sfid = 0x321;
    transmit_message.tx_efid = 0x01;
    transmit_message.tx_ft = CAN_FT_DATA;
    transmit_message.tx_ff = CAN_FF_STANDARD;
    transmit_message.tx_dlen = 1;
    printf("please press the Tamper key to transmit data!\r\n");
    while(1){
        /* waiting for the Tamper key pressed */
        while(0 == gd_eval_key_state_get(KEY_TAMPER)){
            /* if transmit_number is 0x10, set it to 0x00 */
            if(transmit_number == 0x10){
                transmit_number = 0x00;
            }else{
                transmit_message.tx_data[0] = transmit_number++;
                printf("transmit data: %x\r\n", transmit_message.tx_data[0]);
                /* transmit message */
                can_message_transmit(CANX, &transmit_message);
                delay();
                /* waiting for Tamper key up */
                while(0 == gd_eval_key_state_get(KEY_TAMPER));
            }
        }
        if(SET == receive_flag){
            gd_eval_led_toggle(LED1);
            receive_flag = RESET;
            printf("recive data: %x\r\n", receive_message.rx_data[0]);
        }
    } 
}

/*!
    \brief      initialize CAN and filter
    \param[in]  can_parameter
      \arg        can_parameter_struct
    \param[in]  can_filter
      \arg        can_filter_parameter_struct
    \param[out] none
    \retval     none
*/
void can_networking_init(can_parameter_struct can_parameter, can_filter_parameter_struct can_filter)
{
    /* initialize CAN register */
    can_deinit(CANX);
    
    /* initialize CAN */
    can_parameter.time_triggered=DISABLE;
    can_parameter.auto_bus_off_recovery=DISABLE;
    can_parameter.auto_wake_up=DISABLE;
    can_parameter.auto_retrans=DISABLE;
    can_parameter.rec_fifo_overwrite=DISABLE;
    can_parameter.trans_fifo_order=DISABLE;
    can_parameter.working_mode=CAN_NORMAL_MODE;
    can_parameter.resync_jump_width=CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1=CAN_BT_BS1_3TQ;
    can_parameter.time_segment_2=CAN_BT_BS2_2TQ;
    /* baudrate 1Mbps */
    can_parameter.prescaler=12;
    can_init(CANX, &can_parameter);

    /* initialize filter */
#ifdef  CAN0_USED
    /* CAN0 filter number */
    can_filter.filter_number=0;
#else
    /* CAN1 filter number */
    can_filter.filter_number=15;
#endif
    /* initialize filter */    
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x0000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x0000;
    can_filter.filter_mask_low = 0x0000;  
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable=ENABLE;
    can_filter_init(&can_filter);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
#ifdef  CAN0_USED
    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_RX0_IRQn,0,0);
#else
    /* configure CAN1 NVIC */
    nvic_irq_enable(CAN1_RX0_IRQn,0,0);
#endif

}

/*!
    \brief      delay
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay(void)
{
    uint16_t nTime = 0x0000;

    for(nTime = 0; nTime < 0xFFFF; nTime++){
    }
}

/*!
    \brief      configure the leds
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);
}

/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* enable can clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_CAN1);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    
    /* configure CAN0 GPIO */
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5);

    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_6);
    
    /* configure CAN1 GPIO */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_12);
    
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_af_set(GPIOB, GPIO_AF_9, GPIO_PIN_13);
    
}

/* retarget the C library printf function to the usart */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM2, (uint8_t)ch);
    while (RESET == usart_flag_get(EVAL_COM2, USART_FLAG_TBE));
    return ch;
}
