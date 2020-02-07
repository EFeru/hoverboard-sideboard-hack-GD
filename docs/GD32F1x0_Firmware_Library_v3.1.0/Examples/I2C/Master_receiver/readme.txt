/*!
    \file  readme.txt
    \brief description of the master receiver
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows the I2C 
programming mode in master receiving mode. In this demo,BOARD_I2C is operated as master 
receiver, and the SCL line and SDA line of BOARD_I2C interface are controled by the I/O 
pin PB10 and PB11(or PB6 and PB7) respectively.

  This demo shows the receiving data process of the master. And it will store 
the received data in the i2c_receiver array.

  This demo doesn't perform the data transfer actually, which is due to no 
specific slave.In the specific application, we must send the correct slave 
address, and the master and the slave may need to be connected by the jumper 
if necessary. When the macro I2C_10BIT_ADDRESS is 1, I2C communicate in 10 bit
addressing mode, otherwise, I2C communicate in 7 bit addressing mode.
