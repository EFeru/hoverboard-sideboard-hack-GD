# hoverboard-sideboard-hack-GD

[![Build Status](https://travis-ci.com/EmanuelFeru/hoverboard-sideboard-hack-GD.svg?token=PqYy6QC9o8nDVwJPGmrv&branch=master)](https://travis-ci.com/EmanuelFeru/hoverboard-sideboard-hack-GD)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=CU2SWN2XV9SCY&currency_code=EUR&source=url)

This repository implements the firmware for the hoveboard sideboards. The hoveboard comes with two identical sideboards that can be flashed with this firmware.

### ► If you have an STM32 sideboard check-out the repo [hoverboard-sideboard-hack-STM](https://github.com/EmanuelFeru/hoverboard-sideboard-hack-STM)

---
## Hardware

The original sideboard hardware supports one 4-pin cable that originally was connected to the hoveboard mainboard. It breaks out GND, 12/15V and USART. Additionally, two ports are used to connect to the LED boards. On the back of the board, two Photo Interrupter Optical Switches can be found, originally used to detect if a human is standing on the hoverboard.
![sideboard](/docs/pictures/sideboard_pinout.png)

The LED boards consist of colored LEDs (blue, red, green, orange) used for design and to inform the user about the current hoverboard state. Below the pinout of the LED boards is shown.
![ledboard](/docs/pictures/ledboard_pinout.png)

The heart of the sideboard is a [GD32F130C6T6](/docs/GD32F130xx-Datasheet_Rev3.3.pdf) with the pinout shown in the follwing figure:
![MCU_pinout](/docs/pictures/MCU_pinout.png)

A very important component of the sideboard is the IMU [MPU-6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/) from Invensense. The [MPU-6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/) determines the board orientation by combining a 3-axis gyroscope and a 3-axis accelerometer on the same silicon die, together with an onboard Digital Motion Processor™ (DMP™), which processes complex 6-axis MotionFusion algorithms. The DMP™ offers many features, such as:
* Accelerometer and Gyroscope raw data output
* Quaternion output
* Temperature output
* Pedometer
* Interrupts on gesture events such as Tap on all axes or Orientation change
* Low-power modes
* Self-test and calibration

The [MPU-6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/) pinout and orientation of axes is shown in the follwing figure:
![MPU6050_pinout](/docs/pictures/MPU6050_pinout.png)

For more details see the [MPU-6050 datasheet](/docs/1_MPU-6000-Datasheet.pdf) and [MPU-6050 registers](/docs/2_MPU-6000-Register-Map.pdf).

---
## Flashing 

On the sideboard, there is a debugging header with GND, 3V3, SWDIO and SWCLK. Connect GND, SWDIO and SWCLK to your ST-Link V2 programmer. The 3V3 can be either obtained by connecting the pin to the ST-Link programmer or powering the sideboard with 12/15V.

If you have never flashed your sideboard before, the MCU is probably locked. To unlock the flash, check-out the wiki page [How to Unlock MCU flash](https://github.com/EmanuelFeru/hoverboard-firmware-hack-FOC/wiki/How-to-Unlock-MCU-flash).

To build and flash choose one of the following methods:

### Method 1: Using PlatformIO

Because this board is not yet supported by [PlatformIO](https://platformio.org/), we have to make two extra steps. These steps can be skipped once the board is supported (see [this thread](https://community.platformio.org/t/build-gd32-project-with-platformio/11944)).

- go to your PlatformIO home folder (Windows: `C:\Users\<user>\.platformio`, Unix/Max: `/home/<user>/.platformio`). Then go into `packages`. If the folder `framework-spl` exists, delete it. 

- unpack the `framework-spl.zip` in the `packages` folder so that the directory structure is now:
```
packages/
| - framework-spl/
| |--  gd32/
| |--  platformio/
| |--  stm32/
| |--  package.json
```
(This folder contains the `GD32F1x0_Firmware_Library_v3.1.0` files)

- open the project folder in the IDE of choice (vscode or Atom)
- press the 'PlatformIO:Build' or the 'PlatformIO:Upload' button (bottom left in vscode).

### Method 2: Using Keil uVision

- in [Keil uVision](https://www.keil.com/download/product/), open the [sideboard-hack.uvproj](/MDK-ARM/)
- if you are asked to install missing packages, click Yes
- click Build Target (or press F7) to build the firmware
- click Load Code (or press F8) to flash the firmware.

### Method 3: Using Ubuntu

- prerequisites: install [ST-Flash utility](https://github.com/texane/stlink).

- open a terminal in the repo check-out folder and type:
```
make
```
- flash the firmware by typing:
```
make flash
```


---
## Example Variants 

This firmware offers currently these variants (selectable in [config.h](/Inc/config.h)):
- **VARIANT_DEBUG**: In this variant the user can interact with sideboard via a Serial Monitor to observe and check the capabilities of the sideboard.
- **VARIANT_HOVERBOARD**: In this variant the sideboard is communicating with the mainboard of a hoverboard using the [FOC firmware repository](https://github.com/EmanuelFeru/hoverboard-firmware-hack-FOC). This Variant is not yet fully tested!

Of course the firmware can be further customized for other needs or projects.

---
## 3D Visualization Demo

By [converting Quaternions to Euler angles](https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles), we can make a [3D visualization example](/docs/sketch_processing/sketch_processing.pde) in [Processing](https://processing.org/) as shown below. For this Demo VARIANT_HOVERBOARD was used.

![sketch_pic](/docs/pictures/sketch_processing_pic.png)


---
## Contributions

Every contribution to this repository is highly appreciated! Feel free to create pull requests to improve this firmware as ultimately you are going to help everyone. 

If you want to donate to keep this firmware updated, please use the link below:

[![paypal](https://www.paypalobjects.com/en_US/NL/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=CU2SWN2XV9SCY&currency_code=EUR&source=url)

---
