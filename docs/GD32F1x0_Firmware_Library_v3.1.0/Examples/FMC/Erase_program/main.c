/*!
    \file  main.c
    \brief main flash program, erase and reprogram
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include "main.h"
#include "gd32f1x0_eval.h"

#define FMC_PAGE_SIZE           ((uint16_t)0x400U)
#define FMC_WRITE_START_ADDR    ((uint32_t)0x08004000U)
#define FMC_WRITE_END_ADDR      ((uint32_t)0x08004800U)

uint32_t *ptrd;
uint32_t address = 0x00U;
uint32_t data0   = 0x01234567U;
uint32_t data1   = 0xd583179bU;
led_typedef_enum lednum = LED4;

/* calculate the number of page to be programmed/erased */
uint32_t PageNum = (FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) / FMC_PAGE_SIZE;
/* calculate the number of page to be programmed/erased */
uint32_t WordNum = ((FMC_WRITE_END_ADDR - FMC_WRITE_START_ADDR) >> 2);

/*!
    \brief      erase fmc pages from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_erase_pages(void)
{
    uint32_t EraseCounter;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    
    /* erase the flash pages */
    for(EraseCounter = 0U; EraseCounter < PageNum; EraseCounter++){
        fmc_page_erase(FMC_WRITE_START_ADDR + (FMC_PAGE_SIZE * EraseCounter));
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    /* lock the main FMC after the erase operation */
    fmc_lock();
}

/*!
    \brief      program fmc word by word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_program(void)
{
    /* unlock the flash program/erase controller */
    fmc_unlock();

    address = FMC_WRITE_START_ADDR;

    /* program flash */
    while(address < FMC_WRITE_END_ADDR){
        fmc_word_program(address, data0);
        address += 4U;
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    /* lock the main FMC after the program operation */
    fmc_lock();
}

#ifdef GD32F170_190
/*!
    \brief      reprogram fmc word by word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_reprogram(void)
{
    /* unlock the flash program/erase controller */
    fmc_unlock();

    address = FMC_WRITE_START_ADDR;

    /* reprogram the previous program address */
    while(address < FMC_WRITE_END_ADDR){
        fmc_word_reprogram(address, data1);
        address += 4U;
        fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }

    /* lock the main FMC after the reprogram operation */
    fmc_lock();
}
#endif /* GD32F170_190 */

/*!
    \brief      check fmc erase result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_erase_pages_check(void)
{
    uint32_t i;

    ptrd = (uint32_t *)FMC_WRITE_START_ADDR;

    /* check flash whether has been erased */
    for(i = 0U; i < WordNum; i++){
        if(0xFFFFFFFFU != (*ptrd)){
            lednum = LED1;
            gd_eval_led_on(lednum);
            break;
        }else{
            ptrd++;
        }
    }
}

/*!
    \brief      check fmc program result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_program_check(void)
{
    uint32_t i;

    ptrd = (uint32_t *)FMC_WRITE_START_ADDR;

    /* check flash whether has been programmed */
    for(i = 0U; i < WordNum; i++){
        if((*ptrd) != data0){
            lednum = LED2;
            gd_eval_led_on(lednum);
            break;
        }else{
            ptrd++;
        }
    }
}

#ifdef GD32F170_190
/*!
    \brief      check fmc reprogram result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_reprogram_check(void)
{
    uint32_t i;

    ptrd = (uint32_t *)FMC_WRITE_START_ADDR;

    /* check reprogram result, only can reprogram corresponding bit value from 1 to 0 */
    for(i = 0U; i < WordNum; i++){
        if(0x01030503U != (*ptrd)){
            lednum = LED3;
            gd_eval_led_on(lednum);
            break;
        }else{
            ptrd++;
        }
    }
}
#endif /* GD32F170_190 */

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize led on the board */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);

    /* step1: erase pages and check if it is successful. If not, light the LED1. */
    fmc_erase_pages();
    fmc_erase_pages_check();

    /* step2: program and check if it is successful. If not, light the LED2. */
    fmc_program();
    fmc_program_check();

    #ifdef GD32F170_190
    /* step3: reprogram and check if it is successful. If not, light the LED3. */
    fmc_reprogram();
    fmc_reprogram_check();
    #endif /* GD32F170_190 */

    /* if all the operations are successful, light the LED4. */
    if(LED4 == lednum){
        gd_eval_led_on(LED4);
    }

    while(1){
    }
}
