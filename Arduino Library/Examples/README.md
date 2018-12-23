# I2C Encoder V2 Arduino Library Examples
--------------------------------------------------------------------------------

## Introduction

Here you can find some examples for Arduino boards.
For more details of the functionality of the board please read the [Datasheet](https://github.com/Fattoresaimon/I2CEncoderV2/blob/master/EncoderI2CV2_v1.5.pdf) 

## Installation

The installation is very simple:
* Download the files **i2cEncoderLibV2.cpp** and **i2cEncoderLibV2.h**
* Put them in the folder where you have you arduino **.ino** source file
* Add the following "include" on the top of the source file:
``` C++
#include <Wire.h>
#include "i2cEncoderLibV2.h"
```

## Connection with Arduino UNO
In the examples the connection with the Arduino UNO is the following:

| I2C Encoder V2 | Arduino UNO |
| ---------- | ---------- |
| - | GND|
| + | 5V|
| SDA | A4 |
| SCL | A5 |
| INT | A3 |

## Connection with ESP32
In the examples the connection with the ESP32 dev board is the following:

| I2C Encoder V2 | ESP32 |
| ---------- | ---------- |
| - | GND|
| + | 3.3V|
| SDA | G21 |
| SCL | G22 |
| INT | G23 |

## Connection with ESP8266
In the examples the connection with the ESP8266 is the following:

| I2C Encoder V2 | ESP8266 |
| ---------- | ---------- |
| - | GND|
| + | 3.3V|
| SDA | 4 |
| SCL | 5 |
| INT | 16 |

The INT pin is used as GPIO, you can change in the code according to your preference.``
