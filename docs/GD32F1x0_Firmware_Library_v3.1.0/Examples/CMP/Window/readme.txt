/*!
    \file  readme.txt
    \brief description of the comparator window demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows how to make 
  an analog watchdog by using comparator 
  peripherals in window mode:
  - The upper threshold is set to VREFINT = 1.22V
  - The lower threshold is set to VREFINT / 2 = 1.22V = 0.610V
  - The input voltage is configured to be connected to PC1(GD32150R) or PC0(GD32190R) 

  If the input voltage is above the higher threshold, LED1 and LED3 are turned on.
  If the input voltage is under the lower threshold, LED2 and LED4 are turned on.
  If the input voltage is within the thresholds, the MCU remains in deepsleep mode
  and all leds are turned off.

  Connect PA1 to PC1(GD32150R) or PC0(GD32190R).   
