/*!
    \file  readme.txt
    \brief description of the low voltage detector demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This example is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to use low 
voltage detector. If VDD/VDDA is higher or lower than the LVD threshold, then a lvd event will occur.
The event is internally connected to the EXTI line 16 and can generate an interrupt if enabled through
the EXTI registers. Use external power connect 3.3v pin and GND pin of the board , adjust voltage higher 
or lower than the LVD threshold: 2.9v(GD32150R-EVAL) or 4.5v(GD32190R-EVAL), led1 will toggle.
 