/*!
    \file  readme.txt
    \brief description of the ADC resolution demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32190R-EVAL board, it shows how to use 
the function of programmble resolusion. In this demo, 6B resolusion 
is configured. PC0(channel10) is chosen as analog input pin. The ADC 
conversion begins by software, the converted data is printed by USART.

  The analog input pin should configured to analog mode.
  Jump the JP14, JP15 to USART.