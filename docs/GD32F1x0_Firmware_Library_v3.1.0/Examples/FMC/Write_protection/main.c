/*!
    \file  main.c
    \brief main flash pages write protection
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
#include "main.h"

/*!
    \brief      enable some pages' write protection function by configuring option byte
    \param[in]  wp_pages_bitmap: bitmap of pages which need to be enabled write protection function
    \param[out] none
    \retval     none
*/
void fmc_ob_write_protection_enable(uint16_t wp_pages_bitmap)
{
    uint8_t ob_user;
    uint16_t ob_data;
    uint16_t old_wp, new_wp;

    /* unlock the main flash and option byte */
    fmc_unlock();
    ob_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    /* backup the old OB_USER, OB_DATA and OB_WP */
    ob_user = ob_user_get();
    ob_data = ob_data_get();
    old_wp  = ob_write_protection_get();

    /* it's need to do operation just when the pages indicated by wp_pages_bitmap have not been enabled */
    if(0 != (old_wp & wp_pages_bitmap)){
        /* caculate the new write protectiom bitmap */
        new_wp = ((~old_wp) | wp_pages_bitmap);

        /* erase the option byte before modify the content */
        ob_erase();

        /* restore the OB_USER and OB_DATA */
        ob_user_write(ob_user);
        ob_data_program(OB_DATA_ADDR0, (uint8_t)ob_data);
        ob_data_program(OB_DATA_ADDR1, (uint8_t)(ob_data >> 8));

        /* enable the new write protection in option byte */
        ob_write_protection_enable(new_wp);

        /* lock the option byte firstly and then lock the main flash after operation */
        ob_lock();
        fmc_lock();

        /* reload the option byte and generate a system reset */ 
        ob_reset();
    }
}

/*!
    \brief      disable some pages' write protection function by configuring option byte
    \param[in]  wp_pages_bitmap: bitmap of pages which need to be disabled write protection function
    \param[out] none
    \retval     none
*/
void fmc_ob_write_protection_disable(uint16_t wp_pages_bitmap)
{
    uint8_t ob_user;
    uint16_t ob_data;
    uint16_t old_wp, new_wp;

    /* unlock the main flash and option byte */
    fmc_unlock();
    ob_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    /* backup the old OB_USER, OB_DATA and OB_WP */
    ob_user = ob_user_get();
    ob_data = ob_data_get();
    old_wp  = ob_write_protection_get();

    /* it's need to do operation just when the pages indicated by wp_pages_bitmap have been enabled */
    if((old_wp & wp_pages_bitmap) != wp_pages_bitmap){
        /* caculate the new write protectiom bitmap */
        new_wp = ~(old_wp | wp_pages_bitmap);

        /* erase the option byte before modify the content */
        ob_erase();

        /* restore the OB_USER and OB_DATA */
        ob_user_write(ob_user);
        ob_data_program(OB_DATA_ADDR0, (uint8_t)ob_data);
        ob_data_program(OB_DATA_ADDR1, (uint8_t)(ob_data >> 8));

        /* enable the new write protection in option byte */
        ob_write_protection_enable(new_wp);

        /* lock the option byte firstly and then lock the main flash after operation */
        ob_lock();
        fmc_lock();

        /* reload the option byte and generate a system reset */ 
        ob_reset();
    }
}

/*!
    \brief      erase and program flash, meanwhile check the operation result
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fmc_erase_and_program(void)
{
    uint32_t *ptr = (uint32_t *)ERASE_PAGE_START_ADDR;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    /* erase target page */
    fmc_page_erase(ERASE_PAGE_START_ADDR);
    /* check the erase result, light the LED3 if the result is failed */
    if(0xFFFFFFFF != (*ptr)){
        gd_eval_led_on(LED3);

        return;
    }

    /* program target address */
    fmc_word_program(PROGRAM_ADDRESS, PROGRAM_DATA);
    /* check the program result, light the LED3 if the result is failed */
    if(PROGRAM_DATA != (*ptr)){
        gd_eval_led_on(LED3);

        return;
    }

    /* light the LED4 if the erase and program result are both successful */
    gd_eval_led_on(LED4);

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);

    /* lock the main FMC after the operation */
    fmc_lock();
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize the LED1 and LED2 */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);

    /* configure the keys */
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_EXTI);
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_EXTI);
    gd_eval_key_init(KEY_USER, KEY_MODE_EXTI);

    /* check the write protection result and light corresponding LEDs */
    if(WP_ALL_PAGES_BITMAP == ob_write_protection_get()){
        gd_eval_led_on(LED1);
    }else{
        gd_eval_led_on(LED2);
    }

    /* erase and program flash, 
       failure (light LED3) indicates the page is in write protection, 
       success (light LED4) indicates the page is not in write protection */
    fmc_erase_and_program();

    while(1);
}

