/*!
    \file  slcd_seg.c
    \brief source of the slcd segment driver
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "slcd_seg.h"
#include "gd32f1x0_slcd.h"
#include "gd32f1x0_gpio.h"
#include "gd32f1x0_rcu.h"

/* digit SLCD DATA buffer */
uint8_t digit[4];

/* table of the digit code for SLCD */
__I uint32_t numbertable[10]= 
{
/* 0     1     2     3     4 */
0xDD, 0x88, 0x79, 0xE9, 0xAC,
/* 5     6     7     8     9 */
0xE5, 0xF5, 0x89, 0xFD, 0xED
};

static void digit_to_code(uint8_t c);
static void slcd_gpio_config(void);
static void slcd_seg_digit_write(uint8_t ch, uint8_t position , slcd_display_enum type);

/*!
    \brief      convert digit to SLCD code
    \param[in]  the digit to write
    \param[out] none
    \retval     none
*/
static void digit_to_code(uint8_t c)
{
    uint8_t ch = 0;

    /* the *c is a number */
    if (c < 10)
        ch = numbertable[c];

    digit[0] = (uint8_t)((ch) & 0x03);
    digit[1] = (uint8_t)((ch >> 2) & 0x03);
    digit[2] = (uint8_t)((ch >> 4) & 0x03);
    digit[3] = (uint8_t)((ch >> 6) & 0x03);
}

/*!
    \brief      init the GPIO port of SLCD peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void slcd_gpio_config(void)
{
    /* enable the clock of GPIO */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOF);

    /* SLCD GPIO */
    /* configure GPIOA */
    gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
    gpio_af_set(GPIOA,GPIO_AF_11,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
    
    /* configure GPIOB */
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15);
    gpio_af_set(GPIOB,GPIO_AF_11,GPIO_PIN_9|GPIO_PIN_14|GPIO_PIN_15);
    
    /* configure GPIOC */
    gpio_mode_set(GPIOC,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    gpio_af_set(GPIOC,GPIO_AF_11,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);
    
    /* configure GPIOF */
    gpio_mode_set(GPIOF,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    gpio_output_options_set(GPIOF,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    gpio_af_set(GPIOF,GPIO_AF_11,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
}

/*!
    \brief      init the GPIO port of the SLCD and SLCD peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_seg_configuration(void)
{
    uint16_t i;

    /* configure the SLCD GPIO pins */
    slcd_gpio_config();

    /* enable the clock of SLCD */
    rcu_periph_clock_enable(RCU_SLCD);
    /* wait 2 RTC clock to write SLCD register */
    for(i = 0;i < 500;i++);
    /* config the prescaler and the divider of SLCD clock */
    slcd_clock_config(SLCD_PRESCALER_4,SLCD_DIVIDER_19);
    /* SLCD bias voltage select */
    slcd_bias_voltage_select(SLCD_BIAS_1_3);
    /* SLCD duty cycle select */
    slcd_duty_select(SLCD_DUTY_1_4);
    /* SLCD voltage source select */
    slcd_voltage_source_select(SLCD_VOLTAGE_EXTERNAL);
    /* SLCD pulse on duration config */
    slcd_pulse_on_duration_config(SLCD_PULSEON_DURATION_7);
    /* SLCD dead time duration config */
    slcd_dead_time_config(SLCD_DEADTIME_PERIOD_7);
    /* enable the permanent high drive */
    slcd_high_drive_config(ENABLE);
    /* wait for SLCD CFG register synchronization */
    while (!slcd_flag_get(SLCD_FLAG_SYN));
    /* enable SLCD interface */
    slcd_enable();
    /* wait for SLCD controller on flag */
    while (!slcd_flag_get(SLCD_FLAG_ON));
    /* wait for SLCD voltage ready flag */
    while (!slcd_flag_get(SLCD_FLAG_VRDY));
}

/*!
    \brief      this function write one digit to the SLCD DATA register
    \param[in]  ch: the digit to write
    \param[in]  position: position in the SLCD of the digit to write
    \param[out] none
    \retval     none
*/
void slcd_seg_digit_display(uint8_t ch, uint8_t position)
{
    /* wait the last SLCD DATA update request finished */
    while(slcd_flag_get(SLCD_FLAG_UPR));

    /* SLCD write a char */
    slcd_seg_digit_write(ch, position , INTEGER);

    /* request SLCD DATA update */
    slcd_data_update_request();
}

/*!
    \brief      this function write a integer(6 digits) to SLCD DATA register
    \param[in]  num: number to send to SLCD(0-999999)
    \param[out] none
    \retval     none
*/
void slcd_seg_number_display(uint32_t num)
{
    uint8_t i = 0x00 , length , ch[6];

    ch[5] = num / 100000;
    ch[4] = (num % 100000) / 10000;
    ch[3] = (num % 10000) / 1000;
    ch[2] = (num % 1000) / 100;
    ch[1] = (num % 100 )/ 10;
    ch[0] = num % 10;
    
    if(ch[5]){
        length = 6;
    }else if(ch[4]){
        length = 5;
    }else if(ch[3]){
        length = 4;
    }else if(ch[2]){
        length = 3;
    }else if(ch[1]){
        length = 2;
    }else{
        length = 1;
    }

    slcd_seg_clear_all();
    /* wait the last SLCD DATA update request finished */
    while(slcd_flag_get(SLCD_FLAG_UPR));

    /* send the string character one by one to SLCD */
    while (i < length){
        /* display one digit on SLCD */
        slcd_seg_digit_write( ch[i] , 6-i , INTEGER );
        /* increment the digit counter */
        i++;
    }

    /* request SLCD DATA update */
    slcd_data_update_request();
}

/*!
    \brief      this function write a float number(6 digits which has 2 decimal) to SLCD DATA register
    \param[in]  num: number to send to SLCD
    \param[out] none
    \retval     none
*/
void slcd_seg_floatnumber_display(float num)
{
    uint8_t i = 0x00 , length , ch[6];
    uint32_t temp;

    temp = (uint32_t)(num * 100);
    ch[5] = temp / 100000;
    ch[4] = (temp % 100000) / 10000;
    ch[3] = (temp % 10000) / 1000;
    ch[2] = (temp % 1000) / 100;
    ch[1] = (temp % 100) / 10;
    ch[0] = temp % 10;

    if(ch[5]){
        length = 6;
    }else if(ch[4]){
        length = 5;
    }else if(ch[3]){
        length = 4;
    }else{
        length = 3;
    }

    slcd_seg_clear_all();
    /* wait the last SLCD DATA update request finished */
    while(slcd_flag_get(SLCD_FLAG_UPR));

    /* send the string character one by one to SLCD */
    while (i < length){
        /* display one digit on SLCD */
        slcd_seg_digit_write(ch[i], 6-i , FLOAT);
        /* increment the digit counter */
        i++;
    }

    /* request SLCD DATA update */
    slcd_data_update_request();
}

/*!
    \brief      this function write time to SLCD DATA register
    \param[in]  num: number to send to SLCD
    \param[out] none
    \retval     none
*/
void slcd_seg_time_display(uint8_t hour, uint8_t minute , uint8_t second)
{
    uint8_t i = 0x00 , ch[6];

    ch[0] = hour/10;
    ch[1] = hour%10;
    ch[2] = minute/10;
    ch[3] = minute%10;
    ch[4] = second/10;
    ch[5] = second%10;

    /* wait the last SLCD DATA update request finished */
    while(slcd_flag_get(SLCD_FLAG_UPR));

    /* send the string character one by one to SLCD */
    while (i < 6){
        /* display one digit on SLCD */
        slcd_seg_digit_write(ch[i], i+1 , TIME);
        /* increment the digit counter */
        i++;
    }

    /* request SLCD DATA update */
    slcd_data_update_request();
}

/*!
    \brief      this function write a digit to SLCD DATA register
    \param[in]  ch: the digit to write
    \param[in]  position: position in the SLCD of the digit to write,which can be 1..6
    \param[in]  type: the type of the data
    \param[out] none
    \retval     none
*/
static void slcd_seg_digit_write(uint8_t ch, uint8_t position , slcd_display_enum type )
{
    /* convert ASCii to SLCD digit or char */
    digit_to_code(ch);

    switch (position){
    case 6:
        /* clear the corresponding segments (COM0..COM3, SEG30..31) */
        SLCD_DATA0 &= (uint32_t)(0x3FFFFFFF);
        SLCD_DATA1 &= (uint32_t)(0x3FFFFFFF);
        SLCD_DATA2 &= (uint32_t)(0x3FFFFFFF);
        SLCD_DATA3 &= (uint32_t)(0x3FFFFFFF);

        /* write the colon of the time (COM0, SEG27) */
        if(type == TIME)
        SLCD_DATA0 |= (uint32_t)((uint32_t)0x01 << 31);
        /* write the corresponding segments (COM0..COM3, SEG30..31) */
        SLCD_DATA0 |= (uint32_t)(digit[0] << 30);
        SLCD_DATA1 |= (uint32_t)(digit[1] << 30);
        SLCD_DATA2 |= (uint32_t)(digit[2] << 30);
        SLCD_DATA3 |= (uint32_t)(digit[3] << 30);
        break;

    case 5:
        /* clear the corresponding segments (COM0..COM3, SEG28..29) */
        SLCD_DATA0 &= (uint32_t)(0xCFFFFFFF);
        SLCD_DATA1 &= (uint32_t)(0xCFFFFFFF);
        SLCD_DATA2 &= (uint32_t)(0xCFFFFFFF);
        SLCD_DATA3 &= (uint32_t)(0xCFFFFFFF);

        /* write the corresponding segments (COM0..COM3, SEG28..29) */
        SLCD_DATA0 |= (uint32_t)(digit[0] << 28);
        SLCD_DATA1 |= (uint32_t)(digit[1] << 28);
        SLCD_DATA2 |= (uint32_t)(digit[2] << 28);
        SLCD_DATA3 |= (uint32_t)(digit[3] << 28);
        break;

    case 4:
        /* clear the corresponding segments (COM0..COM3, SEG26..27) */
        SLCD_DATA0 &= (uint32_t)(0xF3FFFFFF);
        SLCD_DATA1 &= (uint32_t)(0xF3FFFFFF);
        SLCD_DATA2 &= (uint32_t)(0xF3FFFFFF);
        SLCD_DATA3 &= (uint32_t)(0xF3FFFFFF);

        /* write the point (COM0, SEG27) */
        if(type == FLOAT)
        SLCD_DATA0 |= (uint32_t)(0x01 << 27);
        /* write the corresponding segments (COM0..COM3, SEG26..27) */
        SLCD_DATA0 |= (uint32_t)(digit[0] << 26);
        SLCD_DATA1 |= (uint32_t)(digit[1] << 26);
        SLCD_DATA2 |= (uint32_t)(digit[2] << 26);
        SLCD_DATA3 |= (uint32_t)(digit[3] << 26);
        break;

    case 3:
        /* clear the corresponding segments (COM0..COM3, SEG24..25) */
        SLCD_DATA0 &= (uint32_t)(0xFCFFFFFF);
        SLCD_DATA1 &= (uint32_t)(0xFCFFFFFF);
        SLCD_DATA2 &= (uint32_t)(0xFCFFFFFF);
        SLCD_DATA3 &= (uint32_t)(0xFCFFFFFF);

        /* write the colon of the time (COM0, SEG25) */
        if(type == TIME)
        SLCD_DATA0 |= (uint32_t)(0x01 << 25);
        /* write the corresponding segments (COM0..COM3, SEG24..25) */
        SLCD_DATA0 |= (uint32_t)(digit[0] << 24);
        SLCD_DATA1 |= (uint32_t)(digit[1] << 24);
        SLCD_DATA2 |= (uint32_t)(digit[2] << 24);
        SLCD_DATA3 |= (uint32_t)(digit[3] << 24);
        break;

    case 2:
        /* clear the corresponding segments (COM0..COM3, SEG22..23) */
        SLCD_DATA0 &= (uint32_t)(0xFF3FFFFF);
        SLCD_DATA1 &= (uint32_t)(0xFF3FFFFF);
        SLCD_DATA2 &= (uint32_t)(0xFF3FFFFF);
        SLCD_DATA3 &= (uint32_t)(0xFF3FFFFF);

        /* write the corresponding segments (COM0..COM3, SEG22..23) */
        SLCD_DATA0 |= (uint32_t)(digit[0] << 22);
        SLCD_DATA1 |= (uint32_t)(digit[1] << 22);
        SLCD_DATA2 |= (uint32_t)(digit[2] << 22);
        SLCD_DATA3 |= (uint32_t)(digit[3] << 22);
        break;


    case 1:
        /* clear the corresponding segments (COM0..COM3, SEG14..15) */
        SLCD_DATA0 &= (uint32_t)(0xFFFF3FFF);
        SLCD_DATA1 &= (uint32_t)(0xFFFF3FFF);
        SLCD_DATA2 &= (uint32_t)(0xFFFF3FFF);
        SLCD_DATA3 &= (uint32_t)(0xFFFF3FFF);

        /* write the corresponding segments (COM0..COM3, SEG14..15) */
        SLCD_DATA0 |= (uint32_t)(digit[0] << 14);
        SLCD_DATA1 |= (uint32_t)(digit[1] << 14);
        SLCD_DATA2 |= (uint32_t)(digit[2] << 14);
        SLCD_DATA3 |= (uint32_t)(digit[3] << 14);
        break;
    }
}

/*!
    \brief      this function clear data in the SLCD DATA register
    \param[in]  position: position in the SLCD of the digit to write,which can be 1..6
    \param[out] none
    \retval     none
*/
void slcd_seg_digit_clear(uint8_t position)
{
    switch (position){
    case 6:
        /* clear the corresponding segments (COM0..COM3, SEG30..31) */
        SLCD_DATA0 &= (uint32_t)(0x3FFFFFFF);
        SLCD_DATA1 &= (uint32_t)(0x3FFFFFFF);
        SLCD_DATA2 &= (uint32_t)(0x3FFFFFFF);
        SLCD_DATA3 &= (uint32_t)(0x3FFFFFFF);
        break;

    case 5:
        /* clear the corresponding segments (COM0..COM3, SEG28..29) */
        SLCD_DATA0 &= (uint32_t)(0xCFFFFFFF);
        SLCD_DATA1 &= (uint32_t)(0xCFFFFFFF);
        SLCD_DATA2 &= (uint32_t)(0xCFFFFFFF);
        SLCD_DATA3 &= (uint32_t)(0xCFFFFFFF);
        break;

    case 4:
        /* clear the corresponding segments (COM0..COM3, SEG26..27) */
        SLCD_DATA0 &= (uint32_t)(0xF3FFFFFF);
        SLCD_DATA1 &= (uint32_t)(0xF3FFFFFF);
        SLCD_DATA2 &= (uint32_t)(0xF3FFFFFF);
        SLCD_DATA3 &= (uint32_t)(0xF3FFFFFF);
        break;

    case 3:
        /* clear the corresponding segments (COM0..COM3, SEG24..25) */
        SLCD_DATA0 &= (uint32_t)(0xFCFFFFFF);
        SLCD_DATA1 &= (uint32_t)(0xFCFFFFFF);
        SLCD_DATA2 &= (uint32_t)(0xFCFFFFFF);
        SLCD_DATA3 &= (uint32_t)(0xFCFFFFFF);
        break;

    case 2:
        /* clear the corresponding segments (COM0..COM3, SEG22..23) */
        SLCD_DATA0 &= (uint32_t)(0xFF3FFFFF);
        SLCD_DATA1 &= (uint32_t)(0xFF3FFFFF);
        SLCD_DATA2 &= (uint32_t)(0xFF3FFFFF);
        SLCD_DATA3 &= (uint32_t)(0xFF3FFFFF);
        break;

    case 1:
        /* clear the corresponding segments (COM0..COM3, SEG14..15) */
        SLCD_DATA0 &= (uint32_t)(0xFFFF3FFF);
        SLCD_DATA1 &= (uint32_t)(0xFFFF3FFF);
        SLCD_DATA2 &= (uint32_t)(0xFFFF3FFF);
        SLCD_DATA3 &= (uint32_t)(0xFFFF3FFF);
        break;
    }
}

/*!
    \brief      this function clear all the SLCD DATA register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void slcd_seg_clear_all(void)
{
    SLCD_DATA0 = 0x0000;
    SLCD_DATA1 = 0x0000;
    SLCD_DATA2 = 0x0000;
    SLCD_DATA3 = 0x0000;
}
