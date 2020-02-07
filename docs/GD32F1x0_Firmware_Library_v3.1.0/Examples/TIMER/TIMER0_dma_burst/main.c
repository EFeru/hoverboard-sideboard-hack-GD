/*!
    \file  main.c
    \brief TIMER0 dma burst demo
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

#define TIMER0_DMATB           ((uint32_t)0x040012C4C)
uint16_t buffer[8] = {99,199,299,399,499,599,699,799};

void gpio_config(void);
void timer_config(void);
void dma_config(void);

/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */
void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);

    /*configure PA8(TIMER0 CH0) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_8);

    /*configure PA9(TIMER0 CH1) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_9);

    /*configure PA10(TIMER0 CH2) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_10);

    /*configure PA11(TIMER0 CH3) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_11);
}

/**
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void dma_config(void)
{
    dma_parameter_struct dma_init_struct;

    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

    /* initialize DMA channel4 */
    dma_deinit(DMA_CH4);

    /* DMA channel4 initialize */
    dma_deinit(DMA_CH4);
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr  = (uint32_t)buffer;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_init_struct.number       = 8;
    dma_init_struct.periph_addr  = (uint32_t)TIMER0_DMATB;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA_CH4,dma_init_struct);

    /* configure DMA mode */
    dma_circulation_enable(DMA_CH4);
    dma_memory_to_memory_disable(DMA_CH4);

    /* enable DMA channel4 */
    dma_channel_enable(DMA_CH4);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
    /* TIMER0 DMA transfer example ----------------------------------------------------
    TIMER0CLK = 72MHz, prescaler = 72 
    TIMER0 counter clock = systemcoreclock/72 = 1MHz.

      The objective is to configure TIMER0 channel 0~3(PA8~PA11) to generate PWM signal.
    capture compare register 0~3 are to be updated twice per circle. On the first update 
    DMA request, data1 is transferred to CH0CV, data2 is transferred to CH1CV, data3 is 
    transferred to CH2CV, data4 is transferred to CH3CV and duty cycle(10%,20%,30%,40%). 
    On the second update DMA request, data5 is transferred to CH0CV, data6 is transferred 
    to CH1CV, data7 is transferred to CH2CV, data8 is transferred to CH3CV and duty cycle
    (50%,60%,70%,80%). 
    -----------------------------------------------------------------------------------*/
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);

    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_HIGH;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);

    /* CH0 configuration in PWM0 mode */
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,buffer[0]);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);

    /* CH1 configuration in PWM0 mode */
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,buffer[0]);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

    /* CH2 configuration in PWM0 mode */
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,buffer[0]);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM0 mode */
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,buffer[0]);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

    /* TIMER0 primary output enable */
    timer_primary_output_config(TIMER0,ENABLE);

    /* TIMER0 update DMA request enable */
    timer_dma_transfer_config(TIMER0,TIMER_DMACFG_DMATA_CH0CV,TIMER_DMACFG_DMATC_4TRANSFER);
    timer_dma_enable(TIMER0,TIMER_DMA_UPD);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);

    /* TIMER0 counter enable */
    timer_enable(TIMER0);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gpio_config(); 
    dma_config();
    timer_config();

    while (1);
}
