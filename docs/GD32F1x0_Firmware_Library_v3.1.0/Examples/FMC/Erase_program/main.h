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

/* erase fmc pages from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR */
void fmc_erase_pages(void);
/* program fmc word by word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR */
void fmc_program(void);
#ifdef GD32F170_190
/* reprogram fmc word by word from FMC_WRITE_START_ADDR to FMC_WRITE_END_ADDR */
void fmc_reprogram(void);
#endif /* GD32F170_190 */
/* check fmc erase result */
void fmc_erase_pages_check(void);
/* check fmc program result */
void fmc_program_check(void);
#ifdef GD32F170_190
/* check fmc reprogram result */
void fmc_reprogram_check(void);
#endif /* GD32F170_190 */

#endif

