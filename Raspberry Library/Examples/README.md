# I2C Encoder V2 Python library Examples
--------------------------------------------------------------------------------

## Introduction

Here you can find the library and some examples for Raspberry Pi.
For more details of the functionality of the board please read the [Datasheet](https://github.com/Fattoresaimon/I2CEncoderV2/blob/master/EncoderI2CV2_v1.5.pdf) 

## Installation

There are two versions of the library. The library under the folder [smbus](https://github.com/Fattoresaimon/I2CEncoderV2/tree/master/Raspberry%20Library/smbus)  uses the library smbus, while the library under the folder  [smbus2](https://github.com/Fattoresaimon/I2CEncoderV2/tree/master/Raspberry%20Library/smbus2) uses the library  [smbus2](https://github.com/kplindegaard/smbus2).
The smbus2 is better optimized for the I2C transaction respect to the smbus. 

The installation is very simple:

* Download the file **i2cEncoderLibV2.py**
* Put the file in the folder where you have the source files.
* Import the library in your source files
``` python
import i2cEncoderLibV2
```



## Connection with Arduino UNO
In the examples the connection with the Arduino UNO is the following:

| I2C Encoder V2 | Raspberry Pi3 NAME | Raspberry Pi3 Pin#  |
| ---------- | ---------- | ---------- |
| - | Ground | 9 or 6 |
| + | 3.3v | 1|
| SDA | GPIO02 | 3 |
| SCL | GPIO03 | 4|
| INT | GPIO04 | 5 |

The INT pin is used as GPIO, you can change in the code according to your preference.
