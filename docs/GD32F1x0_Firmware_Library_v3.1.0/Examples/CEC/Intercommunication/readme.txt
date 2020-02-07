/*!
    \file  readme.txt
    \brief description of CEC intercommunication
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/
  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
configure and use the CEC peripheral to receive and transmit messages. In this 
demo, the same program is loaded to two GD32150R-EVAL/GD32190R-EVAL boards, 
then they are communicated through connecting respective CEC line(PB8) and GND.
When press the buttons(K3 or K4) on one board, LED1 and LED2 will blink once.
  Connect two GD32150R-EVAL/GD32190R-EVAL boards through CEC line(PB8) and GND.
