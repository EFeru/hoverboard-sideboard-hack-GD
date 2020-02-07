/*!
    \file  readme.txt
    \brief description of the UASRT receiver timeout
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL, it shows how to use USART receiver 
timeout function.  
  Firstly, the USART sends "a usart receive timeout test example!" to the hyperterminal
and then loops waiting for receiving data from the hyperterminal. If you do not enter any
data more than 3 seconds, USART will send strings to the hyperterminal what you have entered
and start a new round of waiting.
