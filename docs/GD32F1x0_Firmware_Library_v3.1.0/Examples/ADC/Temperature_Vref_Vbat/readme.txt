/*!
    \file  readme.txt
    \brief description of the ADC TIMER trigger injected channel
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to 
use the ADC to convert analog signal to digital data.The ADC is configured 
in dependent mode, inner channel16(temperature sensor channel),channel17(VREF channel) 
and channel18(VBAT/2 channel) are chosen as analog input pin. The ADC clock 
is configured to 12MHz.
  As the AD convertion begins by software,the converted data in the ADC_IDTRx register
,where the x is 0 to 2.    
 
  The analog input pin should configured to AN mode and the ADC clock should 
below 14MHz for GD32150R(28MHz for GD32190R-EVAL).
