/*!
    \file  readme.txt
    \brief description of the CRC calculate demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
configure the CRC module. In this demo, 8 CRC codes are calculated for a 32-bit
data by configuring various control bits in the CRC control register.

  If the test passes, the success_flag variable equals 0x1 and turn on the LED1.
