/*!
    \file  readme.txt
    \brief description of the DBG demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows that, when the 
DBG_CTL0_TM1_HOLD bit in DBG control register(DBG_CTL0) is set and the core halted,
the TIMER1 counter stop counting and the PWM outputs of all channels are stopped as
well.It's benefit for debuging.

  The TIMER1 counter clock used is 1MHz.
The Three Duty cycles are computed as the following description: 
The channel 1 duty cycle is set to 25%
The channel 2 duty cycle is set to 50%
The channel 3 duty cycle is set to 75%

  Connect the TIMER1 pins to an oscilloscope and monitor the different waveforms:
- TIMER1_CH1  pin (PB3)
- TIMER1_CH2  pin (PB10)
- TIMER1_CH3  pin (PB11)

  For Keil,in debug mode,when the core is stopped,update the register window for TIMER1,
you will see that the count value will not change. And at the same time,the PWM outputs of 
all the channels of TIMER1 will be stopped. 

