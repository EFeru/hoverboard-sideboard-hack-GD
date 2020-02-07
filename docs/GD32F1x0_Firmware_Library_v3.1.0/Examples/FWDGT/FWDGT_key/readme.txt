/*!
    \file  readme.txt
    \brief description of the FWDGT_key example
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
reload the FWDGT counter at regulate period using the EXTI interrupt. The FWDGT 
timeout is set to 1s (the timeout may varies due to IRC40K frequency dispersion).
 
  An EXTI is connected to a specific GPIO pin and configured to generate an interrupt
on its falling edge: when the EXTI Line interrupt is triggered (by pressing the Key 
push-button on the board), the corresponding interrupt is served. In the ISR, FWDGT 
counter is reloaded). As a result, when the FWDGT counter is reloaded, which prevents 
any FWDGT reset, LED1 remain illuminated.   
  
  If the EXTI Line interrupt does not occur, the FWDGT counter is not reloaded before
the FWDGT counter reaches 00h, and the FWDGT reset. If the FWDGT reset is generated, LED1 
is turned off with the system reset.
  
  In this example the system clock is set to 72 MHz.
