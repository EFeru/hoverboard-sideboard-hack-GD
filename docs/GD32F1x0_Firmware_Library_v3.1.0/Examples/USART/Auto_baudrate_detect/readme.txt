/*!
    \file  readme.txt
    \brief description of the auto baudrate detect
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to use the USART 
to detect and automatically set the USARTx_BRR value based on the reception of 
one character. 
  In this demo, firstly, the USART sends "\n\rUSART auto baudrate detection example\n\r"
to the hyperterminal USART_ABDM_FTOF is set as the auto baud rate detection mode. If auto
baud rate detection is completed without error, LED1 is on.
   You can send any character be in a start 10xxxxxx frame format 1(eg.0x0a or 0x01)
to implement the auto baud rate detection.
