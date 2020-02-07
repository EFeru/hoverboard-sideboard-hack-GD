/*!
    \file  readme.txt
    \brief description of the ADC TIMER trigger injected channel
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/ GD32190R-EVAL board, it shows how to
convert ADC inserted group channels continuously using TIM1 external trigger.The
inserted group length is 4, the scan mode is set, every compare event will trigger
ADC convert all the channels in the inserted group.The ADC clock is configured to 
12MHz(below 14MHz or 28MHz).
