/*!
    \file  readme.txt
    \brief description of the comparator pwm signal control demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
  control PWM output by using comparator output

  - CMP channel0 is configured as following:
  - Inverting input is internally connected to VREFINT = 1.22V
  - Non Inverting input is connected to PC1(GD32150R)/PC0(GD32190R)
  - Output is internally connected to TIMER1 OCREFCLR (output compare reference clear)

  - While PC1/PC0 is lower than VREFINT (1.22V), PWM signal is displayed on PB3 
  - While PC1/PC0 is higher than VREFINT, PB3 is displayed in low level.

  Connect PA1 to PC1(GD32150R)/PC0(GD32190R).