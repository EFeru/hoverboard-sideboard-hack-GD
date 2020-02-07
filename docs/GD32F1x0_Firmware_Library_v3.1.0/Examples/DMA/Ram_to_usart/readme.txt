/*!
    \file  readme.txt
    \brief description of DMA ram to usart
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This example is based on the GD32150R-EVAL/ GD32190R-EVAL board, it provides a description 
of how to use DMA channel1/ channel3 to transfer data from RAM memory to USART transmit data 
register.The start of transfer is triggered by software. At the end of the transfer, a 
transfer complete interrupt is generated since it is enabled. If the DMA transfer
operation is finished correctly, data stored array welcome[] will be transfered 
to a serial port tool by USART and LED1 light up. 
