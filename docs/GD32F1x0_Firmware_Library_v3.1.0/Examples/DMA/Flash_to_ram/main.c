/*!
    \file  main.c
    \brief transfer data from FLASH to RAM
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

#define ACCESSSUM                        1024
#define FMC_PAGE_SIZE                    ((uint16_t)0x400)
#define BANK1_WRITE_START_ADDR           ((uint32_t)0x08004000)
#define BANK1_WRITE_END_ADDR             ((uint32_t)0x08004800)

void rcu_config(void);
void nvic_config(void);
void led_config(void);

__IO ErrStatus accessflag = SUCCESS;
uint32_t destdata[256];
uint32_t *ptrd;
__IO uint32_t int_num = 0;
uint32_t erasenum = 0x00, address = 0x00 ,wperror = 0,wperror2 = 0;
uint32_t transdata = 0x3210ABCD;
__IO uint32_t pagenum = 0x00;
volatile fmc_state_enum fmcstatus = FMC_READY;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t i;
    dma_parameter_struct dma_init_struct;
    /* system clocks configuration */
    rcu_config();
         
    /* NVIC configuration */
    nvic_config();
    
    led_config() ;

    /* unlock the flash bank1 program erase controller */
    fmc_unlock();
    
    /* define the number of page to be erased */
    pagenum = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FMC_PAGE_SIZE;
    
    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_PGERR | FMC_FLAG_WPERR | FMC_FLAG_END);
    
    /* erase the flash pages */
    for(erasenum = 0; erasenum < pagenum; erasenum++){
          fmcstatus = fmc_page_erase(BANK1_WRITE_START_ADDR + (FMC_PAGE_SIZE * erasenum));
          wperror += (fmcstatus == FMC_WPERR);
          fmc_flag_clear(FMC_FLAG_PGERR | FMC_FLAG_WPERR | FMC_FLAG_END);
    }

    /* unlock the flash bank1 program erase controller */
    fmc_lock();
    
    ptrd = (uint32_t*)BANK1_WRITE_START_ADDR;
    for(i = 0; i < 256; i++){
        if(0xFFFFFFFF != *ptrd){ 
            accessflag = ERROR;
            break;
        }
        ptrd++;
    }

    /* unlock the flash bank1 program erase controller */
    fmc_unlock();
    
    /* define the number of page to be erased */
    pagenum = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FMC_PAGE_SIZE;
    
    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_PGERR | FMC_FLAG_WPERR | FMC_FLAG_END);    
    
    /* program flash bank1 */
    address = BANK1_WRITE_START_ADDR;
    wperror2 = 0;
    while(address < BANK1_WRITE_END_ADDR){
        fmcstatus = fmc_word_program(address, transdata);
        address = address + 4; 
        wperror2 += (FMC_WPERR == fmcstatus);
        fmc_flag_clear(FMC_FLAG_PGERR | FMC_FLAG_WPERR | FMC_FLAG_END);
    }
    
    fmc_lock();

    /* DMA channel0 initialize */
    dma_deinit(DMA_CH0);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)destdata;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ACCESSSUM;
    dma_init_struct.periph_addr = (uint32_t)BANK1_WRITE_START_ADDR;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH0,dma_init_struct);
    /* DMA channel0 mode configuration */
    dma_circulation_disable(DMA_CH0);
    dma_memory_to_memory_enable(DMA_CH0);
    /* DMA channel0 interrupt configuration */
    dma_interrupt_enable(DMA_CH0, DMA_INT_FTF);
    /* enable DMA transfer */
    dma_channel_enable(DMA_CH0);

    /* wait DMA interrupt */
    for(i = 0; i < 10000; i++){
        if(int_num)
        break;
    }    
    
    /* compare destdata with transdata */
    ptrd = destdata;
    for(i = 0; i < 256; i++){
        if(transdata != *ptrd)
        { 
            accessflag = ERROR;
            break;
        }
        ptrd++;
    }
    
    /* transfer sucess */
    if(accessflag != ERROR){
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED3);
        gd_eval_led_on(LED2);
        gd_eval_led_on(LED4);
    }else{
        /* transfer fail */
        gd_eval_led_off(LED2);
        gd_eval_led_off(LED4);
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED3);
    }
   
    while(1);
}
  
/*!
    \brief      configure LED
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init (LED1);
    gd_eval_led_init (LED2);
    gd_eval_led_init (LED3);
    gd_eval_led_init (LED4);
    
    /* LED off */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED3);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED4);
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_irq_enable(DMA_Channel0_IRQn,0,0);
}

