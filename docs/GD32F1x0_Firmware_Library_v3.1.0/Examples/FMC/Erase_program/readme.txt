/*!
    \file  readme.txt
    \brief description of erase_program example
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This example is based on the GD32150R-EVAL/GD32190R-EVAL board, it provides a
description of how to erase and program flash memory and reprogram without previous
earse.
 
  In erasing operation, a comparison between flash memory and 0xffffffff is done to 
check whether the flash memory has been correctly erased. If the result is wrong, 
LED1 will be on.

  In programming operation, a comparison between flash memory and target data is 
done to check whether the flash memory has been correctly programmed. If the result 
is wrong, LED2 will be on.

  In reprogramming operation (only exists in GD32190R-EVAL board), corresponding bit 
value will be changed from 1 to 0, If the result is wrong, LED3 will be on.
  
  If all the three operations are successful, LED4 will be on.
 