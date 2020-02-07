/*!
    \file  rc5_encode.c
    \brief the rc5 infrared encoder file
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "rc5_encode.h"
#include "ir_decode.h"

/* rc5 high level definition*/
#define  RC5_HIGH_STATE   ((uint8_t)0x02)
/* rc5 low level definition*/
#define  RC5_LOW_STATE    ((uint8_t)0x01)

rc5_ctrl_enum rc5_ctrl1 = RC5_CTRL_RESET;

uint8_t      rc5_real_frame_length       = 14;
uint8_t      rc5_global_frame_length     = 64;
uint16_t     rc5_frame_binary_format     = 0;
uint32_t     rc5_frame_manchester_format = 0;
uint8_t      send_operation_ready        = 0;
__IO uint8_t send_operation_completed    = 1;
uint8_t      bits_sent_counter           = 0;

uint8_t address_index = 0;
uint8_t instruction_index = 0;

static uint16_t rc5_bin_frame_generation(uint8_t rc5_address, uint8_t rc5_instruction, rc5_ctrl_enum rc5_ctrl);
static uint32_t rc5_manchester_convert(uint16_t rc5_binary_frame_format);

/*!
    \brief      init hardware (ips used) for rc5 generation
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rc5_encode_init( void )
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    /* TIMER15 clock enable */
    rcu_periph_clock_enable(RCU_TIMER15);
    /* TIMER16 clock enable */
    rcu_periph_clock_enable(RCU_TIMER16);

    {
        /* IR_encode */
        /* PB9 IR_OUT pin configuration: output */
        gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
        gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
        gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_9);
    }

    {
        {
            /* deinit TIMER16 */
            timer_deinit(TIMER16);

            /* time base = 36Khz */
            /* time base configuration for TIMER16 */
            timer_initpara.prescaler         = 0x00;
            timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
            timer_initpara.counterdirection  = TIMER_COUNTER_UP;
            timer_initpara.period            = 1999;
            timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
            timer_initpara.repetitioncounter = 0;
            timer_init(TIMER16,&timer_initpara);

            /* prescaler configuration */
            timer_prescaler_config(TIMER16,0,TIMER_PSC_RELOAD_NOW);
          
            /* output compare timing mode configuration: channel 0N */
            timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
            timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
            timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
            timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_LOW;
            timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
            timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
            timer_channel_output_config(TIMER16,TIMER_CH_0,&timer_ocintpara);

            timer_channel_output_pulse_value_config(TIMER16, TIMER_CH_0, (2000/4-1));
            timer_channel_output_mode_config(TIMER16, TIMER_CH_0, TIMER_OC_MODE_PWM1);
            timer_channel_output_shadow_config(TIMER16, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);

            /* TIMER16 enable */
            timer_enable(TIMER16);
            /* enable the TIMER16 channel1 output to be connected internly to the IRTIMER */
            timer_primary_output_config(TIMER16, ENABLE);
        }
  
        {
            /* deinit TIMER15 */
            timer_deinit(TIMER15);

            /* elementary period 889 us */
            /* time base configuration for TIMER15 */
            timer_initpara.prescaler         = 1;
            timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
            timer_initpara.counterdirection  = TIMER_COUNTER_UP;
            timer_initpara.period            = 64008 /2 - 1;
            timer_initpara.clockdivision     = 0;
            timer_initpara.repetitioncounter = 0;
            timer_init(TIMER15,&timer_initpara);

            /* duty cycle = 25% */
            /* channel 1 configuration in timing mode */
            timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
            timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
            timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
            timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
            timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
            timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
            timer_channel_output_config(TIMER15, TIMER_CH_0, &timer_ocintpara);

            timer_channel_output_pulse_value_config(TIMER15, TIMER_CH_0, (64008 /2 /4));
            timer_channel_output_mode_config(TIMER15, TIMER_CH_0, TIMER_OC_MODE_TIMING);

            /* enable the TIMER15 interrupt */
            nvic_priority_group_set(NVIC_PRIGROUP_PRE0_SUB4);
            nvic_irq_enable(TIMER15_IRQn, 0, 0);
      
            /* TIMER15 main output enable */
            timer_primary_output_config(TIMER15, ENABLE);
            /* TIMER15 INT disable */
            timer_interrupt_disable(TIMER15, TIMER_INT_UP);
            /* TIMER15 disable */
            timer_disable(TIMER15);
        }
    }
}

/*!
    \brief      generate and send the rc5 frame
    \param[in]  rc5_address: the rc5 device destination
    \param[in]  rc5_instruction: the rc5 command instruction
    \param[in]  rc5_ctrl: the rc5 control bit
    \param[out] none
    \retval     none
*/
void rc5_encode_send_frame(uint8_t rc5_address, uint8_t rc5_instruction, rc5_ctrl_enum rc5_ctrl)
{
    uint16_t rc5_frame_binary_format = 0;
    
    /* generate a binary format of the frame */
    rc5_frame_binary_format = rc5_bin_frame_generation(rc5_address, rc5_instruction, rc5_ctrl);
    
    /* generate a manchester format of the frame */
    rc5_frame_manchester_format = rc5_manchester_convert(rc5_frame_binary_format);
    
    /* set the send operation ready flag to indicate that the frame is ready to be sent */
    send_operation_ready = 1;
    
    /* TIMER15 INT enable */
    timer_interrupt_enable(TIMER15, TIMER_INT_UP);
  
    /* enable TIMER15 */
    timer_enable(TIMER15);
}

/*!
    \brief      send by hardware manchester format rc5 frame
    \param[in]  rc5_manchester_frame_format: the rc5 frame in manchester format
    \param[out] none
    \retval     none
*/
void rc5_encode_signal_generate( uint32_t rc5_manchester_frame_format )
{
    uint8_t bit_msg = 0;
    
    if((1 == send_operation_ready ) && ((rc5_global_frame_length * 2) >= bits_sent_counter)){
        send_operation_completed = 0x00;
        bit_msg = (uint8_t)((rc5_manchester_frame_format >> bits_sent_counter) & 1);
        
        if(1 == bit_msg){
            timer_channel_output_mode_config(TIMER15, TIMER_CH_0, TIMER_OC_MODE_HIGH);
        }else{
            timer_channel_output_mode_config(TIMER15, TIMER_CH_0, TIMER_OC_MODE_LOW);
        }
        bits_sent_counter++;
    }else{
        send_operation_completed = 0x01;

        /* TIMER15 INT disable */
        timer_interrupt_disable(TIMER15, TIMER_INT_UP);
        timer_disable(TIMER15);

        send_operation_ready = 0;
        bits_sent_counter = 0;
        timer_channel_output_mode_config(TIMER15, TIMER_CH_0, TIMER_OC_MODE_LOW);
    }
}

/*!
    \brief      generate the binary format of the rc5 frame
    \param[in]  rc5_address: select the device adress
    \param[in]  rc5_instruction: select the device instruction
    \param[in]  rc5_ctrl: select the device control bit status
    \param[out] none
    \retval     binary format of the rc5 frame
*/
static uint16_t rc5_bin_frame_generation(uint8_t rc5_address, uint8_t rc5_instruction, rc5_ctrl_enum rc5_ctrl)
{
    uint16_t star1 = 0x2000;
    uint16_t star2 = 0x1000;
    uint16_t addr = 0;
    
    while(0x00 == send_operation_completed); 
  
    /* check if instruction is 128-bit length */
    if(64 <= rc5_instruction){
        /* reset field bit: command is 7-bit length */
        star2 = 0;
        /* keep the lowest 6 bits of the command */
        rc5_instruction &= 0x003F;
    }
    /* instruction is 64-bit length */
    else{
        /* set field bit: command is 6-bit length */
        star2 = 0x1000;
    }

    send_operation_ready        = 0;
    rc5_frame_manchester_format = 0;
    rc5_frame_binary_format     = 0;
    addr = ((uint16_t)(rc5_address))<<6;
    rc5_frame_binary_format = (star1) | (star2) | (rc5_ctrl) | (addr) | (rc5_instruction);
    return(rc5_frame_binary_format);
}

/*!
    \brief      convert the rc5 frame from binary to manchester format
    \param[in]  rc5_binary_frame_format: the rc5 frame in binary format
    \param[out] none
    \retval     the rc5 frame in manchester format
*/
static uint32_t rc5_manchester_convert(uint16_t rc5_binary_frame_format)
{
    uint8_t i=0;
    uint16_t mask = 1;
    uint16_t bit_format = 0;
    uint32_t converted_msg =0;
    
    for(i = 0; i < rc5_real_frame_length; i++){
        bit_format = ((((uint16_t)(rc5_binary_frame_format))>>i)& mask)<<i;
        converted_msg = converted_msg << 2;

        if(0 != bit_format){
            /* manchester 1 -|_  */
            converted_msg |= RC5_HIGH_STATE;
        }else{
            /* manchester 0 _|-  */
            converted_msg |= RC5_LOW_STATE;
        }
    }
    return(converted_msg);
}
