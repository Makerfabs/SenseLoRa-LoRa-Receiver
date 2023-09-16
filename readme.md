# SENSELORA LORA RECEIVER

```c++
/*
Version:		V1.0
Author:			Vincent
Create Date:	2023/9/16
Note:

*/
```
# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

# How To Use

## Arduino Complier Option

**If you have any questions，such as how to install the development board, how to download the code, how to install the library. Please refer to :[Makerfabs_FAQ](https://github.com/Makerfabs/Makerfabs_FAQ)**

- Please disconnect from the electrical equipment before burning!
- Do not burn with alternating current!
- Open Arduino IDE
- Open boards Manager, search "RP2040"
- Install board:  Arduino Mbed OS RP2040 Boards(Version 4.0.4)

Arduino libraries:

- RP2040_SD 1.0.1  
- RadioLib 4.6.0  
- Adafruit_GFX_Library 1.11.5  
- Adafruit_BusIO 1.14.1  
- Adafruit_SSD1306 2.5.1  


## Data Analysis

Once the device is plugged in, run data_analysis.py and the serial port will be found automatically. In addition, the json data uploaded by the device is classified according to ID and stored into different CSV files.

Make sure that no other serial device is connected, or modify the Python program yourself.

Environment:

- Python      3.11.2
- pyserial    3.5

