/*

Must change pins_arduino.h
File may in this place:
C:\Users\vincent\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.4\variants\RASPBERRY_PI_PICO\pins_arduino.h

// Wire
// #define PIN_WIRE_SDA        (4u)
// #define PIN_WIRE_SCL        (5u)

// Change By Vincent
#define PIN_WIRE_SDA        (6u)
#define PIN_WIRE_SCL        (7u)

*/

// Arduino Mbed OS RP2040 Boards version 4.0.4
// 使用库 SPI 在文件夹： C:\Users\vincent\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.4\libraries\SPI (legacy)
// 使用 1.0.1  版本的库 RP2040_SD 在文件夹： C:\Users\vincent\Documents\Arduino\libraries\RP2040_SD
// 使用 4.6.0  版本的库 RadioLib 在文件夹： C:\Users\vincent\Documents\Arduino\libraries\RadioLib
// 使用库 Wire 在文件夹： C:\Users\vincent\AppData\Local\Arduino15\packages\arduino\hardware\mbed_rp2040\4.0.4\libraries\Wire (legacy)
// 使用 1.11.5  版本的库 Adafruit_GFX_Library 在文件夹： C:\Users\vincent\Documents\Arduino\libraries\Adafruit_GFX_Library
// 使用 1.14.1  版本的库 Adafruit_BusIO 在文件夹： C:\Users\vincent\Documents\Arduino\libraries\Adafruit_BusIO
// 使用 2.5.1  版本的库 Adafruit_SSD1306 在文件夹： C:\Users\vincent\Documents\Arduino\libraries\Adafruit_SSD1306

#include "config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_DIO1, SPI, SPISettings());
File root;
File myFile;
long unsigned int runtime = 0;
int count = 0;
int sd_flag = 0;
int screen_rotation = 2;
int button_flag = 0;

int last_rssi = 0;
String oled_str = "No Massage";
int flesh_flag = 0;

void setup(void)
{
    Serial.begin(115200);

    pinMode(LORA_CS, OUTPUT);
    pinMode(SD_CS, OUTPUT);

    pinMode(PIN_SPI_MISO, INPUT);
    pinMode(PIN_SPI_MOSI, OUTPUT);
    pinMode(PIN_SPI_SCK, OUTPUT);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    delay(2000);
    Serial.println(F("Hello! SenseLoRa LoRa Receiver"));

    oled_init();
    sd_flag = sd_init();
    lora_init();

    while (0)
    {
        lora_send_task("AAAA");
        delay(10000);
    }

    runtime = millis();
    attachInterrupt(BUTTON_PIN, button_func, FALLING); // CHANGE FALLING RISING
    flesh_flag = 1;
}

void loop()
{
    if (button_flag == 1)
    {
        if (screen_rotation == 2)
            screen_rotation = 0;
        else
            screen_rotation = 2;
        display.setRotation(screen_rotation);
        Serial.println("Rotation OLED");
        button_flag = 0;

        flesh_flag = 1;
    }

    if (flesh_flag == 1)
    {
        oled_display();
        flesh_flag = 0;
    }

    lora_node_general();
    if ((millis() - runtime) > 5000)
    {
        // record_msg("loop");
        // log_read();
        runtime = millis();
    }
}

void lora_init()
{
    SD_CS_OFF;
    LORA_CS_ON;

    int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SX127X_SYNC_WORD, OUTPUT_POWER, PREAMBLE_LEN, GAIN);

    if (state == ERR_NONE)
    {
        Serial.println(F("Lora init success!"));
    }
    else
    {
        Serial.print(F("Lora init failed, code "));
        Serial.println(state);
        while (true)
            ;
    }
}

void lora_node_general()
{
    // Serial.print(F("[SX1278] Waiting for incoming transmission ... "));

    String str;
    int state = radio.receive(str);

    if (state == ERR_NONE)
    {
        int rssi = radio.getRSSI();
        // // packet was successfully received
        // Serial.println(F("Get lora message!"));

        // // print the data of the packet
        // Serial.println(F("[SX1278] Data:\t\t\t"));
        Serial.println(str);

        // // print the RSSI (Received Signal Strength Indicator)
        // // of the last received packet
        // Serial.print(F("[SX1278] RSSI:\t\t\t"));
        // Serial.print(rssi);
        // Serial.println(F(" dBm"));

        // // print the SNR (Signal-to-Noise Ratio)
        // // of the last received packet
        // Serial.print(F("[SX1278] SNR:\t\t\t"));
        // Serial.print(radio.getSNR());
        // Serial.println(F(" dB"));

        // // print frequency error
        // // of the last received packet
        // Serial.print(F("[SX1278] Frequency error:\t"));
        // Serial.print(radio.getFrequencyError());
        // Serial.println(F(" Hz"));

        String temp = "";
        temp = temp + "Num:" + ++count;
        record_msg(temp);
        record_msg(str);

        oled_str = str;
        last_rssi = rssi;
        flesh_flag = 1;
    }
    else if (state == ERR_RX_TIMEOUT)
    {
        // timeout occurred while waiting for a packet
        // Serial.println(F("timeout!"));
    }
    else if (state == ERR_CRC_MISMATCH)
    {
        // packet was received, but is malformed
        Serial.println(F("CRC error!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
}

int sd_init()
{
    LORA_CS_OFF;
    SD_CS_ON;

    int fail_count = 0;

    while (1)
    {
        if (fail_count > 2)
            return 0;
        if (!SD.begin(SD_CS))
        {
            SD.end();
            Serial.println("SD init failed!");
            fail_count++;
            delay(500);
            continue;
        }
        else
        {
            Serial.println("SD init done.");
            record_msg("New Restart.......");
            return 1;
        }
    }
}

void record_msg(String data)
{
    if (!sd_flag)
        return;
    LORA_CS_OFF;
    SD_CS_ON;

    File myFile = SD.open(LOG_FILE, FILE_WRITE);

    myFile.println(data.c_str());
    myFile.close();

    SD_CS_OFF;
    LORA_CS_ON;
}

void log_read()
{
    if (!sd_flag)
        return;
    LORA_CS_OFF;
    SD_CS_ON;

    Serial.println("log_read()");

    File myFile = SD.open(LOG_FILE, FILE_READ);
    while (myFile.available())
    {
        Serial.write(myFile.read());
    }

    // close the file:
    myFile.close();

    SD_CS_OFF;
    LORA_CS_ON;
}

void oled_init()
{
    Wire.begin();
    Wire.setClock(100000UL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    Serial.println("SSD1306 found");

    display.setRotation(screen_rotation);
    display.clearDisplay();
    for (int16_t x = 0; x < display.width(); x += 1) // SCREEN_WIDTH
    {
        for (int16_t y = 0; y < display.height(); y += 1) //
        {
            display.drawPixel(x, y, SSD1306_WHITE);
        }
        delay(1);
    }
    display.display();

    delay(2000);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2); // Draw 2X-scale text
    display.setCursor(0, 0);
    display.println("SenseLoRa");
    display.println("LoRa");
    display.println("Receiver");
    display.display();

    display.setTextSize(1);
}

void button_func()
{
    button_flag = 1;
}

//

void lora_send_task(String data)
{
    Serial.println(data);
    int state = radio.transmit(data.c_str());
    if (state == ERR_NONE)
    {
        // the packet was successfully transmitted
        Serial.println(F(" success!"));

        // print measured data rate
        Serial.print(F("[SX1276] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
    }
    else if (state == ERR_PACKET_TOO_LONG)
    {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
    }
    else if (state == ERR_TX_TIMEOUT)
    {
        // timeout occurred while transmitting packet
        Serial.println(F("timeout!"));
    }
    else
    {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
}

void oled_display()
{
    String temp = "";
    temp = temp + "Num:" + count;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(temp);
    display.println((String) " | " + last_rssi + "dbm");
    display.println(oled_str);
    display.display();
}
