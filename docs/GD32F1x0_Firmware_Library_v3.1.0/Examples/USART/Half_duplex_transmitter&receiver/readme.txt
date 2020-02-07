/*!
    \file  readme.txt
    \brief description of the USART HalfDuplex
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it provides a basic communication
USART0 whith USART1 in Half-Duplex mode.
  The transmitter_buffer0 and transmitter_buffer1 are sent by USART0 and USART1 respectively and 
displayed in HyperTerminal. If the data received by USART0 is equal to transmitter_buffer1 and the
data received by USART1 is equal to transmitter_buffer0, LED1 and LED2 is turned on.

  connect  USART0_Tx(PA9) to USART1_Tx(PA2)
  

