/*!
    \file  readme.txt
    \brief description of the can network communication demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

    This demo is based on the GD32190R-EVAL board, it shows how to configure the CAN 
  peripheral to send and receive CAN frames in normal mode.The frames are sent and print
  transmit data by pressing Tamper Key push button. When the frames are received, the receive
  data will be printed and the LED1 will toggle.
 
    Connect JP8 to CAN1
    This example is tested with at least two GD32190R-EVAL boards. The same program example is 
  loaded in all boards and connect CAN_L and CAN_H pin of CAN0 or CAN1 to bus for sending 
  and receiving frames.

  User can select CAN0 or CAN1 cell using the private defines in main.c.


