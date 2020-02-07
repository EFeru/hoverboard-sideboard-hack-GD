/*!
    \file  readme.txt
    \brief description of the dual can communication demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32190R-EVAL board, it shows how to configure the 
CAN0 and CAN1 peripherals to send and receive CAN frames in normal mode. Pressing 
Wakeup button or Tamper button:

  When Tamper button is pressed, CAN0 sends a message to CAN1 and print it.
when CAN1 receives correctly this message,the receive data will be printed
and LED4 toggles a time. 

  When Wakeup button is pressed, CAN1 sends a message to CAN0 and print it.
when CAN0 receives correctly this message,the receive data will be printed
and LED3 toggles a time.     

  User can select one from the preconfigured CAN baud rates from the private 
defines in main.c.These baudrates is correct only when the system clock frequency 
is 72M.

  Connect JP8 to CAN1
  Connect JP4 CAN_L to JP3 CAN_L
  Connect JP4 CAN_H to JP3 CAN_H
