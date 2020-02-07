/*!
    \file  readme.txt
    \brief description of the ADC analog watchdog
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/ GD32190R-EVAL board, it shows how to
use the ADC analog watchdog to guard continuously an ADC channel.The ADC is configured 
in continuous mode, PC1(GD32150R-EVAL) or PC0(GD32190R-EVAL) is chosen as analog
input pin.The ADC clock is configured to 12MHz(below 14MHz or 28MHz).

  When the channel11(channel10) converted value is over the programmed analog 
watchdog high threshold (value 0x0A00) or below the analog watchdog low threshold
(value 0x0400),an AWE interrupt will occur,and LED2 will turn on.  

  The analog input pin should configured to AN mode and the ADC clock should 
below 14MHz for GD32150R(28MHz for GD32190R-EVAL).
