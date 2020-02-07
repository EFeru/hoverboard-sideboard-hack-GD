/*!
    \file  readme.txt
    \brief description of the deepsleep wakeup through exti demo
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This example is based on the GD32150R-EVAL/GD32190R-EVAL board, it shows pmu how to
enter deepsleep mode and wakeup it. Press wakeup key to enter deepsleep mode ,led stop flashing.
When you press tamper key to generate an exti interrupt , mcu will be wakeuped from deepsleep mode ,
led sparks again. But the led sparks slower, because at this time IRC8M is the system clock.
 