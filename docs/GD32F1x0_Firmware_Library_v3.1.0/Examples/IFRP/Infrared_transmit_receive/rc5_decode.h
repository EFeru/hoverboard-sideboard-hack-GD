/*!
    \file  rc5_decode.h
    \brief the header file of rc5 infrared decoding
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#ifndef RC5_DECODE_H
#define RC5_DECODE_H

#include "gd32f1x0.h"

/* rc5 frame structure */
typedef struct
{   
    __IO uint8_t field_bit;       /*!< field bit */   
    __IO uint8_t toggle_bit;      /*!< toggle bit field */
    __IO uint8_t address;         /*!< address field */ 
    __IO uint8_t command;         /*!< command field */ 
} rc5_frame_struct;

/* rc5 packet structure */
typedef struct
{
    __IO uint16_t data;          /*!< rc5 data */ 
    __IO uint8_t status;         /*!< rc5 status */
    __IO uint8_t last_bit;       /*!< rc5 last bit */
    __IO uint8_t bit_count;      /*!< rc5 bit count */
} trc5_packet_struct;

/* rc5 last bit type enum */
enum rc5_last_bit_type
{
    RC5_ZER,
    RC5_ONE,
    RC5_NAN,
    RC5_INV
};

typedef enum rc5_last_bit_type trc5_last_bit_type;

#define RC5_1T_TIME                          0x00
#define RC5_2T_TIME                          0x01
#define RC5_WRONG_TIME                       0xFF
#define RC5_TIME_OUT_US                      3600

/* half bit period */
#define RC5_T_US                             900

/* tolerance time */
#define RC5_T_TOLERANCE_US                   270
#define RC5_NUMBER_OF_VALID_PULSE_LENGTH     2

/* total bits */
#define RC5_PACKET_BIT_COUNT                 13

/* packet struct for reception */
#define RC5_PACKET_STATUS_EMPTY              (uint16_t)(1<<0)

void menu_rc5decode_func(void);
void rc5_decode_deinit(void);
void rc5_decode_init(void);
void rc5_decode(rc5_frame_struct *rc5_frame);
void rc5_reset_packet(void);
void rc5_data_sampling(uint16_t raw_pulse_length, uint8_t edge);

#endif /* RC5_DECODE_H */
