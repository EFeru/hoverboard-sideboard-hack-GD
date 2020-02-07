/*!
    \file  readme.txt
    \brief description of the TIMER2 input capture demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows TIMER 
peripheral to measure the frequency of an external signal.
  
  The TIMxCLK frequency is set to systemcoreclock 72MHz, the prescaler is 72 so 
the TIMER2 counter clock is 1MHz.
  
  The external signal is connected to TIMER2 CH0 pin(PB4).
  The amplitude of the external signal is better to be set as 3.3V.
  The rising edge is used as active edge.
  The TIMER2 CH0CV is used to compute the frequency value.

  The minimum frequence of the input signal to measure is 16 Hz.
  
  You can use USART to watch the frequency of the input signal.The USART is
configured as follow:
  - BaudRate    = 115200 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
