/*!
    \file  readme.txt
    \brief description of the tamper with timestamp example
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it explains how 
to configure the RTC module. In this demo, RTC peripheral is configured to keep
time and generate the tamper interrupt.
  
  The RTC clock source can be chose to LXTAL or IRC40K by uncomment the corresponding 
define in main file. And the HyperTerminal should be connected to the evaluation 
board via USART0/USART1. The vbat bin on the board should connect to the CR1220 battery.
The tamper key is the tamper0 pin. The tamper0 is configured to used for timestamp function. 
  
  After start-up, the program check if the BKP data register is written to a key 
value. If the value is not correct, the program will ask to set the time value on the 
HyperTerminal. If the key value is correct, the current time and subsecond will be 
displayed on the HyperTerminal.
  
  The RTC module is in the Backup Domain, and is not reset by the system reset or 
power reset. 
  
  When press the tamper key, a tamper0 interrupt will be generated, and the tamper0 event 
will reset the BKP0 register, thus if you press the reset key, on the HyperTerminal it asks
you to set the time again. And if the BKP0 register has been reset, when press the tamper key to
come to the interrupt, LED1, LED2 will toggle. If LED3 toggles, there may be other write
operations to BKP0 register has occured. In this example it's configured to enable timestamp 
function when the tamper0 event occurs, thus the timestamp value is then be print on the 
HyperTerminal to tell the time that tamper0 event occured.