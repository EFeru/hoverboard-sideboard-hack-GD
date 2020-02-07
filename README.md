# hoverboard-sideboard-hack-GD

[![Build Status](link to travis)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=CU2SWN2XV9SCY&currency_code=EUR&source=url)

This repository implements the firmware for the hoveboard sideboards. The hoveboard comes with two identical sideboards that can be flashed with this firmware.

The sideboards comes in two MCU flavours:
- [GD32F130C6T6](/docs/GD32F130xx-Datasheet_Rev3.3.pdf) -> use this repository if you have a GD32 MCU
- [STM32F103C8T6](/docs/stm32f103c8-Datasheet.pdf) -> use my [other repository](link) if you have an STM32 MCU

---
## Hardware

![sideboard](/docs/pictures/sideboard_pinout.png)

The original sideboard hardware supports one 4-pin cable that originally were connected to the hoveboard mainboard. They break out GND, 12/15V and USART. Additionally, two ports are used to connect to the LED boards. On the back of the board, two Photo Interrupter Optical Switches can be found, originally used to detect if a human is standing on the hoverboard.

The [GD32F130C6T6](/docs/GD32F130xx-Datasheet_Rev3.3.pdf) pinout is shown in the follwing figure:
![MCU_pinout](/docs/pictures/MCU_pinout.png)

The sideboards, also called sensor boards, feature an [MPU-6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/) from Invensense. The MPU-6050 determines the board orientation by combining a 3-axis gyroscope and a 3-axis accelerometer on the same silicon die, together with an onboard Digital Motion Processor™ (DMP™), which processes complex 6-axis MotionFusion algorithms. The DMP™ offers many features, such as:
* Accelerometer and Gyroscop raw data output
* Quaternion output
* Temperature output
* Pedometer
* Interrupts on gesture events such as Tap on all axes or Orientation change
* Low-power modes
* Self-test and calibration

The MPU-6050 pinout and orientation of axes is shown in the follwing figure:
![MPU6050_pinout](/docs/pictures/MPU6050_pinout.png)

For more details check-out the [MPU-6050 datasheet](/docs/1_MPU-6000-Datasheet.pdf) and [MPU-6050 registers](/docs/2_MPU-6000-Register-Map.pdf).

---
## Flashing 

On the sideboard, there is a debugging header with GND, 3V3, SWDIO and SWCLK. Connect GND, SWDIO and SWCLK to your ST-Link V2 programmer. The 3V3 can be either obtained by connecting the pin to the ST-Link programmer or powering the sideboard with 12/15V.

If you never flashed your sideboard before, the MCU is probably locked. To unlock the flash, check-out the [wiki page](link).

Then, choose one of the following ways for building and flashing the firmware:

### 1. Using Keil uVision (recommended)

In (Keil uVision)[https://www.keil.com/download/product/], open the [sideboard-hack.uvproj](/MDK-ARM/), click Build Target (or press F7), then click Load Code (or press F8).

### 2. Using Ubuntu

Prerequisites: install [ST-Flash utility](https://github.com/texane/stlink).

To build the firmware, type:
```
make
```
then you can simply flash the firmware by typing:
```
make flash
```

*Note: If someone finds a way to build and flash the GD32 MCU via (Platformio)[https://platformio.org/] please let me know.*


---
## Example Variants 

This firmware offers currently these variants (selectable in (config.h)[/Inc/config.h]):
- **VARIANT_DEBUG**: In this variant the user can interact with sideboard via a Serial Monitor to observe and check the capabilities of the sideboard.
- **VARIANT_HOVERBOARD**: In this variant the sideboard is communicating with the mainboard of a hoverboard using the (FOC firmware repository)[link]. !This Variant is not yet fully tested!

Of course the firmware can be further customized for other needs or projects.

--
## 3D Visualization Demo

By calculating the Euler orientation angles from Quaternions, we can make a simple 3D visualization example (see [sketch](/docs/sketch_processing/sketch_processing.pde)) in [Processing](https://processing.org/) as shown below:

![sketch_pic](/docs/pictures/sketch_processing_pic.png)


--
##Contributions

Every contribution to this repository is highly appriciated! Feel free to create pull requests to improve this firmware as ultimately you are going to help everyone. 

If you want to donate to keep this firmware updated, check-out the link below:
[![paypal](https://www.paypalobjects.com/en_US/NL/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=CU2SWN2XV9SCY&currency_code=EUR&source=url)
