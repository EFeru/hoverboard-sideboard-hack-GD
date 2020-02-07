/*!
    \file  main.c
    \brief dual CAN communication in normal mode
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

/* select CAN baudrate */
/* 1MBps */
#define CAN_BAUDRATE  1000
/* 500kBps */
/* #define CAN_BAUDRATE  500 */
/* 250kBps */
/* #define CAN_BAUDRATE  250 */
/* 125kBps */
/* #define CAN_BAUDRATE  125 */
/* 100kBps */ 
/* #define CAN_BAUDRATE  100 */
/* 50kBps */ 
/* #define CAN_BAUDRATE  50 */
/* 20kBps */ 
/* #define CAN_BAUDRATE  20 */

extern can_receive_message_struct receive_message;
FlagStatus can0_receive_flag;
FlagStatus can1_receive_flag;
FlagStatus can0_error_flag;
FlagStatus can1_error_flag;
can_parameter_struct            can_init_parameter;
can_filter_parameter_struct     can_filter_parameter;
can_trasnmit_message_struct transmit_message;

void nvic_config(void);
void led_config(void);
void can_gpio_config(void);
void can_config(can_parameter_struct can_parameter, can_filter_parameter_struct can_filter);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    can0_receive_flag = RESET;
    can1_receive_flag = RESET;
    can0_error_flag = RESET;
    can1_error_flag = RESET;
    
    /* configure GPIO */
    can_gpio_config();
    
    /* configure NVIC */
    nvic_config();
    
    /* configure USART */
    gd_eval_com_init(EVAL_COM2);
    
    /* configure Wakeup key or Tamper key */
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_GPIO);
    
    printf("\r\nGD32F1x0 dual CAN test, please press Wakeup key or Tamper key to start communication!\r\n");
    /* configure leds */
    led_config();
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED4);
    
    /* initialize CAN and filter */
    can_config(can_init_parameter, can_filter_parameter);
    
    /* enable phy */
    can_phy_enable(CAN0);

    /* enable can receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INTEN_RFNEIE0);
    can_interrupt_enable(CAN1, CAN_INTEN_RFNEIE0);
    
    /* initialize transmit message */
    transmit_message.tx_sfid = 0x300>>1;
    transmit_message.tx_efid = 0x00;
    transmit_message.tx_ft = CAN_FT_DATA;
    transmit_message.tx_ff = CAN_FF_STANDARD;
    transmit_message.tx_dlen = 2;

    while(1){
        /* test whether the Tamper key is pressed */
        if(0 == gd_eval_key_state_get(KEY_TAMPER)){
            transmit_message.tx_data[0] = 0x55;
            transmit_message.tx_data[1] = 0xAA;
            printf("\r\n can0 transmit data:%x,%x", transmit_message.tx_data[0],transmit_message.tx_data[1]);
            /* transmit message */
            can_message_transmit(CAN0, &transmit_message);
            /* waiting for the Tamper key up */
            while(0 == gd_eval_key_state_get(KEY_TAMPER));
        }
        /* test whether the Wakeup key is pressed */
        if(0 == gd_eval_key_state_get(KEY_WAKEUP)){
            transmit_message.tx_data[0] = 0xAA;
            transmit_message.tx_data[1] = 0x55;
            printf("\r\n can1 transmit data:%x,%x", transmit_message.tx_data[0],transmit_message.tx_data[1]);
            /* transmit message */
            can_message_transmit(CAN1, &transmit_message);
            /* waiting for the Wakeup key up */
            while(0 == gd_eval_key_state_get(KEY_WAKEUP));
        }
        /* CAN0 receive data correctly, the received data is printed */
        if(SET == can0_receive_flag){
            can0_receive_flag = RESET;
            printf("\r\n can0 receive data:%x,%x",receive_message.rx_data[0],receive_message.rx_data[1]);
            gd_eval_led_toggle(LED3);
        }
        /* CAN1 receive data correctly, the received data is printed */
        if(SET == can1_receive_flag){
            can1_receive_flag = RESET;
            gd_eval_led_toggle(LED4);
            printf("\r\n can1 receive data:%x,%x",receive_message.rx_data[0],receive_message.rx_data[1]);
        }
        /* CAN0 error */
        if(SET == can0_error_flag){
            can0_error_flag = RESET;
            printf("\r\n can0 communication error");
        }
        /* CAN1 error */
        if(SET == can1_error_flag){
            can1_error_flag = RESET;
            printf("\r\n can1 communication error");
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
void can_config(can_parameter_struct can_parameter, can_filter_parameter_struct can_filter)
{
    /* initialize CAN register */
    can_deinit(CAN0);
    can_deinit(CAN1);
    
    /* initialize CAN parameters */
    can_parameter.time_triggered=DISABLE;
    can_parameter.auto_bus_off_recovery=DISABLE;
    can_parameter.auto_wake_up=DISABLE;
    can_parameter.auto_retrans=DISABLE;
    can_parameter.rec_fifo_overwrite=DISABLE;
    can_parameter.trans_fifo_order=DISABLE;
    can_parameter.working_mode=CAN_NORMAL_MODE;
    can_parameter.resync_jump_width=CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1=CAN_BT_BS1_4TQ;
    can_parameter.time_segment_2=CAN_BT_BS2_3TQ;
    
    /* 1MBps */
#if CAN_BAUDRATE == 1000
    can_parameter.prescaler =9;
    /* 500KBps */
#elif CAN_BAUDRATE == 500 
    can_parameter.prescaler =18;
    /* 250KBps */
#elif CAN_BAUDRATE == 250 
    can_parameter.prescaler =36;
    /* 125KBps */
#elif CAN_BAUDRATE == 125 
    can_parameter.prescaler =72;
    /* 100KBps */
#elif  CAN_BAUDRATE == 100 
    can_parameter.prescaler =90;
    /* 50KBps */
#elif  CAN_BAUDRATE == 50 
    can_parameter.prescaler =180;
    /* 20KBps */
#elif  CAN_BAUDRATE == 20 
    can_parameter.prescaler =450;
#else
    #error "please select list can baudrate in private defines in main.c "
#endif  
    /* initialize CAN */
    can_init(CAN0, &can_parameter);
    can_init(CAN1, &can_parameter);
    
    /* initialize filter */ 
    can_filter.filter_number=0;
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high = 0x3000;
    can_filter.filter_list_low = 0x0000;
    can_filter.filter_mask_high = 0x3000;
    can_filter.filter_mask_low = 0x0000;  
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable=ENABLE;
    
    can_filter_init(&can_filter);
    
    /* CAN1 filter number */
    can_filter.filter_number=15;
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
    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_RX0_IRQn,0,0);

    /* configure CAN1 NVIC */
    nvic_irq_enable(CAN1_RX0_IRQn,1,1);
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
void can_gpio_config(void)
{
    /* enable CAN clock */
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
