/*!
    \file  rc5_decode.c
    \brief the rc5 infrared decoding file
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "rc5_decode.h"
#include "ir_decode.h"

/* logic table for rising edge: every line has values corresponding to previous bit.
   in columns are actual bit values for given bit time. */
const trc5_last_bit_type rc5_logic_table_rising_edge[2][2] =
{
    /* last_bit = ZERO */
    {RC5_ZER ,RC5_INV}, 
    /* last_bit = ONE  */
    {RC5_NAN ,RC5_ZER},
};

/* logic table for falling edge: every line has values corresponding to previous bit. 
   in columns are actual bit values for given bit time. */
const trc5_last_bit_type rc5_logic_table_falling_edge[2][2] =
{
    /* last_bit = ZERO */
    {RC5_NAN ,RC5_ONE},  
    /* last_bit = ONE  */
    {RC5_ONE ,RC5_INV},  
};

/* rc5 frame state */
__IO status_yes_or_no rc5_frame_received = NO; 

/* first empty packet */
__IO trc5_packet_struct rc5_tmp_packet;

/* rc5  bits time definitions */
uint16_t rc5_mint = 0;
uint16_t rc5_maxt = 0;
uint16_t rc5_min2t = 0;
uint16_t rc5_max2t = 0;
uint32_t rc5_data = 0;

/* timer clock */
static uint32_t timer_clk_value_khz = 0; 

static uint8_t rc5_get_pulse_length (uint16_t pulse_length);
static void rc5_modify_last_bit(trc5_last_bit_type bit);
static void rc5_write_bit(uint8_t bit_val);
static uint32_t timer_get_counter_clk_value(void);

/*!
    \brief      de-initializes the peripherals
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rc5_decode_deinit(void)
{ 
    timer_deinit(IR_TIMER);
    gpio_deinit(IR_GPIO_PORT);
}

/*!
    \brief      initialize the rc5 decoder module
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rc5_decode_init(void)
{ 
    __IO uint16_t rc5_time_out = 0;

    /* clock configuration for TIMER */
    rcu_periph_clock_enable(IR_TIMER_CLK);
    /* enable GPIO clock */
    rcu_periph_clock_enable(IR_GPIO_PORT_CLK);

    /* IR_decode  */
    /* GD32150R-EVAL:TIMER2_CH0 PC6 
       GD32190R-EVAL:TIMER2_CH1 PA7
       */
    {
        /* configure the GPIO ports */
        gpio_mode_set(IR_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IR_GPIO_PIN);
        gpio_output_options_set(IR_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,IR_GPIO_PIN);

#if defined(GD32F170_190)
        gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_7);
#endif
    }
  
    /* enable the TIMER global interrupt */
    nvic_irq_enable(IR_TIMER_IRQn, 0, 1);

    {
        timer_ic_parameter_struct timer_icinitpara;
        timer_parameter_struct timer_initpara;

        /* deinit IR_TIMER */
        rcu_periph_clock_enable(IR_TIMER_CLK);

        /* TIMER0  configuration */
        timer_deinit(IR_TIMER);

        /* timer base configuration for timer IR_TIMER */
        timer_initpara.prescaler         = 0x0;
        timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
        timer_initpara.counterdirection  = TIMER_COUNTER_UP;
        timer_initpara.period            = 0x00;
        timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
        timer_initpara.repetitioncounter = 0;
        timer_init(IR_TIMER,&timer_initpara);

        /* prescaler configuration */
        timer_prescaler_config(IR_TIMER,TIMER_PRESCALER,TIMER_PSC_RELOAD_NOW);

        /* TIMER2 CH0 input capture configuration */
        timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_FALLING;
        timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
        timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
        timer_icinitpara.icfilter    = 0x0;
        timer_input_pwm_capture_config(IR_TIMER,IR_TIMER_Channel,&timer_icinitpara);
    }

    /* timer clock */
    timer_clk_value_khz = timer_get_counter_clk_value()/1000;
    
    /* select the TIMER input trigger */
#if defined(GD32F130_150)
    timer_input_trigger_source_select(IR_TIMER, TIMER_SMCFG_TRGSEL_CI0FE0);
#elif defined(GD32F170_190)
    timer_input_trigger_source_select(IR_TIMER, TIMER_SMCFG_TRGSEL_CI1FE1);
#endif

    /* select the slave mode: reset mode */
    timer_slave_mode_select(IR_TIMER, TIMER_SLAVE_MODE_RESTART);

    /* configures the TIMER update request interrupt source: counter overflow */
    timer_update_source_config( IR_TIMER, TIMER_UPDATE_SRC_REGULAR );

    rc5_time_out = timer_clk_value_khz * RC5_TIME_OUT_US/1000;
    
    /* set the TIMER auto-reload register for each IR protocol */
    TIMER_CAR(IR_TIMER) = rc5_time_out;

    /* clear flag */
    timer_interrupt_flag_clear(IR_TIMER, TIMER_INT_FLAG_UP | TIMER_INT_FLAG_CH0 | TIMER_INT_FLAG_CH1);

    /* enable the CH1 interrupt request */
    timer_interrupt_enable(IR_TIMER, TIMER_INT_CH1);

    /* enable the CH0 interrupt request */
    timer_interrupt_enable(IR_TIMER, TIMER_INT_CH0);

    /* enable the timer */
    timer_enable(IR_TIMER);

    /* bit time range */
    rc5_mint  = ( RC5_T_US - RC5_T_TOLERANCE_US ) * timer_clk_value_khz / 1000;
    rc5_maxt  = ( RC5_T_US + RC5_T_TOLERANCE_US ) * timer_clk_value_khz / 1000;
    rc5_min2t = ( 2 * RC5_T_US - RC5_T_TOLERANCE_US ) * timer_clk_value_khz / 1000;
    rc5_max2t = ( 2 * RC5_T_US + RC5_T_TOLERANCE_US ) * timer_clk_value_khz / 1000;
    
    /* default state */
    rc5_reset_packet();
}

/*!
    \brief      decode the IR frame when all the frame is received, the rc5_frame_received will equal to YES
    \param[in]  rc5_frame: pointer to rc5_frame_struct structure that contains the IR protocol fields
    \param[out] none
    \retval     none
*/
void rc5_decode(rc5_frame_struct *rc5_frame)
{ 
    /* if frame received */
    if(rc5_frame_received != NO){
        rc5_data = rc5_tmp_packet.data ;

        /* rc5 frame field decoding */
        rc5_frame->address    = (rc5_tmp_packet.data >> 6) & 0x1F;
        rc5_frame->command    = (rc5_tmp_packet.data) & 0x3F; 
        rc5_frame->field_bit  = (rc5_tmp_packet.data >> 12) & 0x1;
        rc5_frame->toggle_bit = (rc5_tmp_packet.data >> 11) & 0x1;
    
        /* check if command ranges between 64 to 127:upper field */
        if(rc5_frame->field_bit == 0x00){
            rc5_frame->command = (1<<6)| rc5_frame->command; 
        }
    
        /* default state */
        rc5_frame_received = NO;
        rc5_reset_packet();
    }  
}

/*!
    \brief      set the incoming packet structure to default state
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rc5_reset_packet(void)
{
    rc5_tmp_packet.data      = 0;
    rc5_tmp_packet.bit_count = RC5_PACKET_BIT_COUNT - 1;
    rc5_tmp_packet.last_bit  = RC5_ONE;
    rc5_tmp_packet.status    = RC5_PACKET_STATUS_EMPTY;
}

/*!
    \brief      identify the rc5 data bits
    \param[in]  raw_pulse_length: low/high pulse duration
    \param[in]  edge: '1' for rising  or '0' for falling edge
    \param[out] none
    \retval     none
*/
void rc5_data_sampling(uint16_t raw_pulse_length, uint8_t edge)
{ 
    uint8_t pulse;
    trc5_last_bit_type tmp_last_bit;
  
    /* decode the pulse length in protocol units */
    pulse = rc5_get_pulse_length(raw_pulse_length);

    /* on rising edge */
    if(1 == edge){ 
        if(pulse <= RC5_2T_TIME){ 
            /* bit determination by the rising edge */
            tmp_last_bit = rc5_logic_table_rising_edge[rc5_tmp_packet.last_bit][pulse];
            rc5_modify_last_bit(tmp_last_bit);
        }else{
            rc5_reset_packet();
        }
    }
    /* on falling edge */
    else {
        /* if this is the first falling edge - don't compute anything */
        if(rc5_tmp_packet.status & RC5_PACKET_STATUS_EMPTY){ 
            rc5_tmp_packet.status &= (uint8_t)~RC5_PACKET_STATUS_EMPTY;
        }else{
            if(pulse <= RC5_2T_TIME){ 
                /* bit determination by the falling edge */
                tmp_last_bit = rc5_logic_table_falling_edge[rc5_tmp_packet.last_bit][pulse];
                rc5_modify_last_bit(tmp_last_bit);
            }else{
                rc5_reset_packet();
            }
        }
    }
}  

/*!
    \brief      convert raw pulse length expressed in timer ticks to protocol bit times
    \param[in]  pulse_length: pulse duration
    \param[out] none
    \retval     bit time value
*/
static uint8_t rc5_get_pulse_length (uint16_t pulse_length)
{ 
    /* valid bit time */
    if((pulse_length > rc5_mint) && (pulse_length < rc5_maxt)){ 
        /* found the length,return the correct value */
        return (RC5_1T_TIME);    
    }else if((pulse_length > rc5_min2t) && (pulse_length < rc5_max2t)){
        /* found the length,return the correct value */
        return (RC5_2T_TIME);
    }
    /* error */
    return RC5_WRONG_TIME;
}

/*!
    \brief      perform checks if the last bit was not incorrect
    \param[in]  bit: where bit can be  RC5_NAN or RC5_INV or RC5_ZER or RC5_ONE
    \param[out] none
    \retval     none
*/
static void rc5_modify_last_bit(trc5_last_bit_type bit)
{
    if(RC5_NAN != bit){
        if(RC5_INV != rc5_tmp_packet.last_bit){ 
            /* restore the last bit */
            rc5_tmp_packet.last_bit = bit;
            /* insert one bit into the rc5 packet */
            rc5_write_bit(rc5_tmp_packet.last_bit);
        }else{
            rc5_reset_packet();
        }
    }
}

/*!
    \brief      insert one bit into the final data word
    \param[in]  bit_val: bit value 'RC5_ONE' or 'RC5_ZER'
    \param[out] none
    \retval     none
*/
static void rc5_write_bit(uint8_t bit_val)
{
    /* first convert rc5 symbols to ones and zeros */
    if(bit_val == RC5_ONE){ 
        bit_val = 1;
    }else if(bit_val == RC5_ZER){
        bit_val = 0;
    }else{
        rc5_reset_packet();
        return;
    } 

    /* write this particular bit to data field */
    rc5_tmp_packet.data |=  bit_val; 
  
    /* test the bit number determined */
    /* if this is not the last bit */
    if(0 != rc5_tmp_packet.bit_count){
        /* shift the data field */
        rc5_tmp_packet.data = rc5_tmp_packet.data << 1;
        /* decrement the bit_count */
        rc5_tmp_packet.bit_count--;
    }else{
        rc5_frame_received = YES;
    }
}

/*!
    \brief      identify TIMER clock
    \param[in]  none
    \param[out] none
    \retval     timer clock
*/
static uint32_t timer_get_counter_clk_value(void)
{
    uint32_t apb_prescaler = 0, apb_frequency = 0;
    uint32_t timer_prescaler = 0;

    rcu_clock_freq_get(CK_APB1);

    /* get the clock prescaler of APB1 */
    apb_prescaler = ((RCU_CFG0>> 8) & 0x7);
    apb_frequency=rcu_clock_freq_get(CK_APB1);

    timer_prescaler = TIMER_PRESCALER;

    /* if APBx clock div >= 4 */
    if(apb_prescaler >= 4){
        return((apb_frequency * 2)/(timer_prescaler + 1));
    }else{
        return(apb_frequency/(timer_prescaler+ 1));
    }
}
