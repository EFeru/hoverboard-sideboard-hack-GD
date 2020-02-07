/*!
    \file  readme.txt
    \brief description of the TIMER1 external trigger demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to
start TIMER peripherals with an external trigger.

  TIMER1 is configured as slave timer for an external trigger connected to 
TIMER1 TI0 pin :
  - The TIMER1 TI0FP0 is used as trigger input
  - Rising edge is used to start the TIMER1: trigger mode.
  - TIMER1 is used PWM0 Mode 
  The starts of the TIMER1 counter are controlled by the external trigger. 

  Connect the TIMER1 CH2 pin(PA2) to an oscilloscope to monitor the waveform.
  Connect the TIMER1 CH0 pin(PA5) to +3V3 on the board as a external trigger with a
rising edge.
