/*!
    \file  main.c
    \brief show how to use TSI to realize key
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
#include <stdlib.h>
#include <stdio.h>

#define THRESHOLD1       0x50
#define THRESHOLD2       0x50

/* the current cycle number array of the channel pin */
uint16_t samplenum[2] = {0,0};

/* reference value sample array of TSI group2 */
uint16_t sample_refnum_array2[20] = {0};
uint16_t sample_refnum_array3[20] = {0};

/* average value of cycles */
uint16_t sample_refnum[2] = {0};

void delay(uint32_t nCount);
void gpio_config(void);
void tsi_config(void);
void led_config(void);
void tsi_transfer_pin(uint32_t TSI_groupx_pin);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int i=0;

    /* TSI peripheral and GPIOB periph clock enable */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_TSI);

    /* PB0   TSI_CHCFG_G2P1     SAMPCAP
       PB1   TSI_CHCFG_G2P2     CHANNEL
       PB2   TSI_CHCFG_G2P3     CHANNEL */

    /* configure the GPIO ports */
    gpio_config();

    /* configure the TSI peripheral */
    tsi_config();

    /* configure the LED */
    led_config();

    /* reference cycle value acquisition and processing */
    for(i=0;i<20;i++){
        /* get charge transfer complete cycle number of group2 pin2 */
        tsi_transfer_pin(TSI_CHCFG_G2P2);
        
        /* check the TSI flag:end of acquisition interrupt */
        if(tsi_interrupt_flag_get(TSI_INT_FLAG_CTC) == (uint8_t)SET){
            /* get charge transfer complete cycle number */
            sample_refnum_array2[i] = tsi_group2_cycle_get();
        }

        /* disable the selected pin as channel pin:pin2 */
        tsi_channel_pin_disable(TSI_CHCFG_G2P2);

        /* get charge transfer complete cycle number of group2 pin3 */
        tsi_transfer_pin(TSI_CHCFG_G2P3);
        if(tsi_interrupt_flag_get(TSI_INT_FLAG_CTC) == (uint8_t)SET){
            sample_refnum_array3[i] = tsi_group2_cycle_get();
        }
        tsi_channel_pin_disable(TSI_CHCFG_G2P3);
        /* delay for a period of time while all banks have been acquired */
        delay(0xFFFF);
    }

    /* sum of sample_refnum_array */
    for(i=1;i<20;i++){
        sample_refnum[0] += sample_refnum_array2[i];
        sample_refnum[1] += sample_refnum_array3[i];
    }

    /* average channel cycle value are obtained */
    sample_refnum[0] = sample_refnum[0]/19;
    sample_refnum[1] = sample_refnum[1]/19;

    while (1){
        /* acquisition group2 pin2 */
        tsi_transfer_pin(TSI_CHCFG_G2P2);
     
        /* check the TSI flag end of acquisition interrupt */
        if(tsi_interrupt_flag_get(TSI_INT_FLAG_CTC) == (uint8_t)SET){
            /* get charge transfer complete cycle number */
            samplenum[0] = tsi_group2_cycle_get();
        }

        /* light LED1 */
        if((sample_refnum[0]-samplenum[0]) > THRESHOLD1){
            /* group2 pin2 is touched */
            gd_eval_led_on(LED1);
        }else{
            gd_eval_led_off(LED1);
        }
        tsi_channel_pin_disable(TSI_CHCFG_G2P2);

        /* acquisition group2 pin3 */
        tsi_transfer_pin(TSI_CHCFG_G2P3);
        if(tsi_interrupt_flag_get(TSI_INT_FLAG_CTC) == (uint8_t)SET){
            samplenum[1] = tsi_group2_cycle_get();
        }

        /* light LED2 */
        if((sample_refnum[1]-samplenum[1]) > THRESHOLD2){
            /* group2 pin3 is touched */
            gd_eval_led_on(LED2);
        }else{
            gd_eval_led_off(LED2);
        }
        tsi_channel_pin_disable(TSI_CHCFG_G2P3);

        /* delay for a period of time while all banks have been acquired */
        delay(0xFFFF);
    }
}

/*!
    \brief      insert a delay time
    \param[in]  nCount: specifies the delay time length
    \param[out] none
    \retval     none
*/
void delay(uint32_t ncount)
{
    for(; ncount != 0; ncount--);
}

/*!
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* GPIO PB0 */
    /* alternate function output open-drain for sampling capacitor IO */
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_0);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_OD,GPIO_OSPEED_2MHZ,GPIO_PIN_0);

    /* GPIO PB1 GPIO PB2 */
    /* alternate function output push-pull for channel and shield IOs */
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_1);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_2MHZ,GPIO_PIN_1);
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_2);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_2MHZ,GPIO_PIN_2);

    /* connect pin to peripheral */
    gpio_af_set(GPIOB,GPIO_AF_3,GPIO_PIN_0);
    gpio_af_set(GPIOB,GPIO_AF_3,GPIO_PIN_1);
    gpio_af_set(GPIOB,GPIO_AF_3,GPIO_PIN_2);
}

/*!
    \brief      configure the TSI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void tsi_config(void)
{
    /* TSI configure */
    tsi_init(TSI_CTCDIV_DIV8,TSI_CHARGE_4CTCLK,TSI_TRANSFER_8CTCLK,TSI_MAXNUM4095);
    tsi_sofeware_mode_config();
    tsi_sample_pin_enable(TSI_SAMPCFG_G2P1);
    tsi_group_enable(TSI_GCTL_GE2);

    /* disable hysteresis mode */
    tsi_hysteresis_off(TSI_PHM_G2P1|TSI_PHM_G2P2|TSI_PHM_G2P3);

    /* enable TSI */
    tsi_enable();
}

/*!
    \brief      LEDs configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    /* initialize the LEDs */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    /* close all of LEDs */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
}

/*!
    \brief      acquisition pin y of group x,x=0..5,y=0..3 
    \param[in]  tsi_groupx_piny:pin y of group x
    \param[out] none
    \retval     none
*/
void tsi_transfer_pin(uint32_t tsi_groupx_piny)
{
    /* configure the TSI Pin Channel Mode */
    tsi_channel_pin_enable(tsi_groupx_piny);

    /* wait capacitors discharge */
    delay(0xD00);

    /* clear both CMCE and CCTCF flags */
    tsi_interrupt_flag_clear(TSI_INT_FLAG_CTC|TSI_INT_FLAG_MNERR);

    /* start a new acquisition */
    tsi_software_start();

    /* wait the specified TSI flag state: MCE or CTCF */
    while(RESET==tsi_interrupt_flag_get(TSI_INT_FLAG_CTC|TSI_INT_FLAG_MNERR));
}

