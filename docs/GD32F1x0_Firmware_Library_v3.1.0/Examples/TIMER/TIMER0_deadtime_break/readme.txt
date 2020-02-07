/*!
    \file  readme.txt
    \brief description of the TIMER0 deadtime break demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to
configure the TIMER0 peripheral to generate complementary signals, to insert a 
defined dead time value, to use the break feature and to lock the desired 
parameters.

  TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 2 ,so
the TIMER0 counter clock used is 36MHz.

  The duty cycle is computed as the following description:
  the channel 0 duty cycle is set to 25% so channel 0N is set to 75%.

  Insert a dead time equal to (63*16*4)/systemcoreclock = 56us

  Configure the break feature, active at Low level(with GND), and using the 
automatic output enable feature.

  Use the Locking parameters level1.

