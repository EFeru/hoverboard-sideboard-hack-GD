/*!
    \file  readme.txt
    \brief description of write_protection example
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This example is based on the GD32150R-EVAL board, it provides a description 
of how to enable and disable the write protection for the embedded flash.
    
  After system start-up, LED1 and LED4 usually are lighted. Meanwhile, LED2 and LED3 usually 
are off. It indicates that no pages of flash are in write protection and all pages can be 
erased/programmed successfully. If not, press the key User to change the board to the normal 
condition.
    
  STEP1: Press the key Wakeup. The LED1 and LED4 will be off and LED2 and LED3 will be on. 
It indicates that some pages are in write protection and erasing/programming on them is failed.

  STEP2: Press the key User. The LED1 and LED4 will be on and LED2 and LED3 will be off. 
It indicates that those pages are out of write protection and erasing/programming on them is successful.

  STEP3: Repeat the STEP1.

  STEP4: Press the key Tamper. The LED2 and LED4 will be on and LED1 and LED3 will be off. 
It indicates that some pages are out of write protection and can be erased/programmed successfully.
But some other pages are still in write protection.

  STEP5: Press the key User. The LED1 and LED4 will be on and LED2 and LED3 will be off. 
It indicates that all pages are out of write protection and erasing/programming on them is successful.

  Note: After testing the example, please ensure to press the key User to disable all pages' 
write protection.


