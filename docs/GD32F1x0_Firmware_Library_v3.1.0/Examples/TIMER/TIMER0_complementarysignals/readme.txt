/*!
    \file  readme.txt
    \brief description of the TIMER0 complementary signals demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
configure the TIMER0 peripheral to generate three complementary TIMER0 signals.
  TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 3600 so the 
TIMER0 counter clock used is 20KHz.

  The Three duty cycles are computed as the following description: 
  The channel 0 duty cycle is set to 25% so channel 0N is set to 75%.
  The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
  The channel 2 duty cycle is set to 75% so channel 2N is set to 25%.   

  Connect the TIMER0 pins to an oscilloscope to monitor the different waveforms:
  - TIMER0_CH0  pin (PA8)
  - TIMER0_CH0N pin (PB13)  
  - TIMER0_CH1  pin (PA9)  
  - TIMER0_CH1N pin (PB14)
  - TIMER0_CH2  pin (PA10)  
  - TIMER0_CH2N pin (PB15)
