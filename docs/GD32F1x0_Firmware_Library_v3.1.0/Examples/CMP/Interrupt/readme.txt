/*!
    \file  readme.txt
    \brief description of the comparator interrupt demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
configure the comparator trigger interrupt using an external interrupt line. 
In this demo,input 3.3V(GD32150R) or 5V(GD32190R)to PA1, EXTI line 21 is 
configured to generate an interrupt on rising edge of the output signal. After 
system start-up, enable comparator and trigger interrupt, then LED2 is on. 

  Connect PA1 to +3V3 or +5V.
