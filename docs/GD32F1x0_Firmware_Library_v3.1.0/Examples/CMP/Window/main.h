/*!
    \file  main.c
    \brief the header file of the main
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

typedef enum
{
    STATE_OVER_THRESHOLD ,
    STATE_WITHIN_THRESHOLD, 
    STATE_UNDER_THRESHOLD 
} cmp_state_enum;

cmp_state_enum check_state(void);

#endif /* MAIN_H */
