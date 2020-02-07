/*!
    \file  readme.txt
    \brief description of the TIMER1 OC active demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to
configure the TIMER peripheral to generate three different signals with three 
different delays.
  
  The TIMxCLK frequency is set to systemcoreclock (72MHz), the prescaler is 36000
so the TIMER1 counter clock is 2KHz.

  And generate 3 signals with 3 different delays:
  TIMER1_CH0 delay = 4000/2000  = 2s
  TIMER1_CH1 delay = 8000/2000  = 4s
  TIMER1_CH2 delay = 12000/2000 = 6s 

  Connect the TIMER1 pins to leds to watch the signal:
  - TIMER1_CH0  pin (PA0)
  - TIMER1_CH1  pin (PA1)
  - TIMER1_CH2  pin (PA2)

  if use GD32150R-EVAL board:
  - LED2  pin (PC11)
  - LED3  pin (PC12)
  - LED4  pin (PD2)

  if use GD32190R-EVAL board:
  - LED2  pin (PA12)
  - LED3  pin (PB6)
  - LED4  pin (PB7)
