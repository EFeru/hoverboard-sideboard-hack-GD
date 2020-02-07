/*!
    \file  main.c
    \brief transfer data from RAM to USART
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
  
#define USART0_DATA_ADDRESS      ((uint32_t)0x40013828)  /* 130_150 device */
#define USART1_DATA_ADDRESS      ((uint32_t)0x40004428)  /* 170_190 device */
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

uint8_t welcome[]="hi,this is a example: RAM_TO_USART by DMA !\n";
__IO uint32_t transfer;

void led_config(void);
void nvic_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
    
    /* initialize LED */
    led_config();
    
    /* all LED off */
    gd_eval_led_off(LED1);

    /*configure DMA interrupt*/
    nvic_config();

#ifdef GD32F130_150

    /* USART configure */
    gd_eval_com_init(EVAL_COM1);
    
    /* initialize DMA channel1 */
    dma_deinit(DMA_CH1);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)welcome;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(welcome);
    dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH1,dma_init_struct);
    
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH1);
    dma_memory_to_memory_disable(DMA_CH1);

    /* USART DMA enable for transmission */
    usart_dma_transmit_config(USART0,USART_DENT_ENABLE);
    
    /* enable DMA transfer complete interrupt */
    dma_interrupt_enable(DMA_CH1,DMA_CHXCTL_FTFIE);
    
    /* enable DMA channel1 */
    dma_channel_enable(DMA_CH1);

#elif defined GD32F170_190
    
    /* USART configure */
    gd_eval_COMinit(EVAL_COM2);
    
    /* initialize DMA channel3 */
    dma_deinit(DMA_CH3);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)welcome;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(welcome);
    dma_init_struct.periph_addr = USART1_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH3,dma_init_struct);
    
    /* configure DMA mode */
    dma_circulation_disable(DMA_CH3);
    dma_memory_to_memory_disable(DMA_CH3);

    /* USART DMA enable for transmission */
    usart_dma_transfer_config(USART1,USART_DENT_ENABLE);
    
    /* enable DMA transfer complete interrupt */
    dma_interrupt_enable(DMA_CH3,DMA_INT_FTF);
    
    /* enable DMA channel3 */
    dma_channel_enable(DMA_CH3);

#endif /* GD32F130_150 */
    
    /* waiting for the transfer to complete*/
    while(0 == transfer);
    
    /* light LED1 */
    gd_eval_led_on(LED1);

    while (1);
}

/*!
    \brief      LEDs configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init (LED1);
}

/*!
    \brief      configure DMA interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
#ifdef GD32F130_150
    nvic_irq_enable(DMA_Channel1_2_IRQn,0,0);
#elif defined GD32F170_190
    nvic_irq_enable(DMA_Channel3_4_IRQn,0,0);
#endif  
}
