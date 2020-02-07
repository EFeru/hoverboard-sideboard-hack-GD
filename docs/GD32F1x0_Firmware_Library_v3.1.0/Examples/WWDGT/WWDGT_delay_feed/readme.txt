/*!
    \file  readme.txt
    \brief description of the WWDGT_delay_feed example
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows the WWDGT
with different counter value and window value make the different WWDGT timeout. As
a result, LED1 and LED2 display a different phenomenon. 
  
  In this demo, the WWDGT counter value is set to 127, the refresh window is set to 
80. In this case the WWDGT timeout is set to 29.2ms(the timeout may varies due to 
IRC40K frequency dispersion), the value in the delay function is set to 25ms, the WWDGT 
counter can reloaded normally,the LED1 is turn off, LED2 is blinking fast.
  
  If the value in the delay function is not set properly, the WWDGT counter is not 
reloaded. As a result, the system reset and the LED1 and LED2 are remain illuminated.
  
  In this example the system clock is set to 72 MHz.
