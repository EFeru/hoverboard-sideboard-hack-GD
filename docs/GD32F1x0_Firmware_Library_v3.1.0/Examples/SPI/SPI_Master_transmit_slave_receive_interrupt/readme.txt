/*!
    \file  readme.txt
    \brief description of the master transmit and slave receive through interrupt demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This example is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows master send data and 
slave receive data use intterupt mode.After the communicate is complete,if receive data  equal 
to send data,led1 turn on,if not led1 turn off.

  Connect SPI0 SCK  PIN(PA5) TO SPI1 SCK  PIN(PB13)
  Connect SPI0 MISO PIN(PA6) TO SPI1 MISO PIN(PB14)
  Connect SPI0 MOSI PIN(PA7) TO SPI1 MOSI PIN(PB15)