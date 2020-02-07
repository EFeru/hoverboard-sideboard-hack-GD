/*!
    \file  readme.txt
    \brief description of the can loopback communication demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32190R-EVAL board, it shows how to set a communication 
with the CAN in loopback mode.

  The example first performs a transmission and a reception of a standard data frame by 
polling at 125 Kbps.If the received frame is successful, the LED1 and LED2 are on. 
otherwise,the LED1 and LED2 are off. Then, an extended data frame is transmitted at 125 Kbps. 
Reception is done in the interrupt handler when the message becomes pending in the FIFO0. If 
the received frame is successful, the LED3 and LED4 are on. otherwise,the LED3 and LED4 are off.
    
  User can select CAN0 or CAN1 cell using the private defines in main.c.
