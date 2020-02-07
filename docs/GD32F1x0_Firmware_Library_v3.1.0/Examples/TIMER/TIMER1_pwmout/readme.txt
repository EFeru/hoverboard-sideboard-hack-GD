/*!
    \file  readme.txt
    \brief description of the TIMER1 PWM output demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to
configure the TIMER peripheral in PWM (Pulse Width Modulation) mode.
  
  The TIMxCLK frequency is set to systemcoreclock (72MHz), the prescaler is 72
so the TIMER1 counter clock is 1MHz.
  
  TIMER1 Channel1 duty cycle = (4000/ 16000)* 100 = 25%.
  TIMER1 Channel2 duty cycle = (8000/ 16000)* 100 = 50%.
  TIMER1 Channel3 duty cycle = (12000/ 16000)* 100 = 75%.

  Connect the TIMER1 pins to an oscilloscope to monitor the different waveforms:
  - TIMER1_CH1  pin (PB3)
  - TIMER1_CH2  pin (PB10)
  - TIMER1_CH3  pin (PB11)
