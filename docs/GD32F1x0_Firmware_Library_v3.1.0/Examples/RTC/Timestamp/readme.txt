/*!
    \file  readme.txt
    \brief description of the timestamp example
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it explains how 
to use the timestamp function of RTC. In this demo, RTC peripheral is configured to keep
time and generate the timestamp interrupt.
  
  The RTC clock source can be chose to LXTAL or IRC40K by uncomment the corresponding 
define in main file. And the HyperTerminal should be connected to the evaluation 
board via USART0/USART1. The vbat bin on the board should connect to the CR1220 battery.
The tamper key is the timestamp pin.
  
  After start-up, the program check if the BKP data register is written to a key 
value. If the value is not correct, the program will ask to set the time value on the 
HyperTerminal. If the key value is correct, the current time and subsecond value
will be displayed on the HyperTerminal. 
  
  Then LED1 is turned on.
  
  The RTC module is in the Backup Domain, and is not reset by the system reset or 
power reset. 
  
  When press the tamper key, a timestamp interrupt will be generated, the current time
then be print on the HyperTerminal, and LED1 toggles.