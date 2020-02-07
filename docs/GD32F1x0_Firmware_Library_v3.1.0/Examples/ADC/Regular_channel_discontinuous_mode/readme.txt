/*!
    \file  readme.txt
    \brief description of the ADC discontinuous mode
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

  This demo is based on the GD32150R-EVAL/ GD32190R-EVAL board, it shows how to
use the ADC discontinuous mode.The ADC is configured in discontinuous mode, group length
is 8,conversion length is 3, using software trigger. Every trigger convert 3 channels.
The ADC clock is configured to 12MHz(below 14MHz or 28MHz).

  You can use the watch window to see the conversion result.
 