#include <SPI.h>
#include <RP2040_SD.h>
#include <RadioLib.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduino.h>

// Pin

#define BUTTON_PIN 2

#define I2C_SDA_PIN 6
#define I2C_SCL_PIN 7

#define PIN_SPI_MISO (16u)
#define PIN_SPI_MOSI (19u)
#define PIN_SPI_SCK (18u)
#define PIN_SPI_SS (17u)

#define LORA_DIO0 21
#define LORA_DIO1 22
#define LORA_DIO2 23
#define LORA_DIO5 24
#define LORA_RST 20

#define LORA_CS 17
#define SD_CS 9

#define LORA_CS_OFF digitalWrite(LORA_CS, HIGH)
#define LORA_CS_ON digitalWrite(LORA_CS, LOW)
#define SD_CS_OFF digitalWrite(SD_CS, HIGH)
#define SD_CS_ON digitalWrite(SD_CS, LOW)

// Oled
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Lora
#define FREQUENCY 868.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 12
#define CODING_RATE 6
#define OUTPUT_POWER 20
#define PREAMBLE_LEN 8
#define GAIN 0

// SD
#define LOG_FILE "Lora_Log.txt"