/*!
    \file  main.h
    \brief the header file of main
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef MAIN_H
#define MAIN_H

/* bit num chosen for this example */
#define WP_PAGES_BIT_1                  (8U)
#define WP_PAGES_BIT_2                  (9U)
#define WP_PAGES_BIT_3                  (10U)
#define WP_ALL_PAGES_BITMAP             (0xFFFFU)

/* construct the bitmap which pages need to be configured the write protection */
#define WRITE_PROTECT_PAGES(bit_num)    (BIT(bit_num))

/* every bit in OB_WP indicates 4 pages if they are in write protection */
#define PAGE_NUM_PER_WP_BIT             (4U)

/* address and data for fmc operation */
#define ERASE_PAGE                      (WP_PAGES_BIT_1 * PAGE_NUM_PER_WP_BIT)
#define ERASE_PAGE_START_ADDR           (0x08000000U + 0x400U * ERASE_PAGE)
#define PROGRAM_ADDRESS                 ERASE_PAGE_START_ADDR
#define PROGRAM_DATA                    (0x12345678U)

/* enable some pages' write protection function by configuring option byte */
void fmc_ob_write_protection_enable(uint16_t wp_pages_bitmap);
/* disable some pages' write protection function by configuring option byte */
void fmc_ob_write_protection_disable(uint16_t wp_pages_bitmap);
/* erase and program flash, meanwhile check the operation result */
void fmc_erase_and_program(void);

#endif

