#include <Arduino.h>
#include <driver/twai.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   7
#define TFT_MOSI  35
#define TFT_SCK   36

#define TOUCH_IRQ   9     // TIRQ
#define TOUCH_MISO  37    // TDO
#define TOUCH_MOSI  35    // TDIN (compartilha com o display)
#define TOUCH_CS    10    // TCS
#define TOUCH_SCK   36    // TCLK (compartilha com o display)

#define CAN_TX 16
#define CAN_RX 15

#define TFT_BL 4

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

void setupCAN()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        (gpio_num_t)CAN_TX,
        (gpio_num_t)CAN_RX,
        TWAI_MODE_NORMAL
    );

    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

void requestRPM()
{
    twai_message_t message;
    message.identifier = 0x7DF;   // Broadcast OBD2
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 8;

    message.data[0] = 0x02; // 2 bytes following
    message.data[1] = 0x01; // Mode 01 (live data)
    message.data[2] = 0x0C; // PID RPM
    message.data[3] = 0x00;
    message.data[4] = 0x00;
    message.data[5] = 0x00;
    message.data[6] = 0x00;
    message.data[7] = 0x00;

    twai_transmit(&message, pdMS_TO_TICKS(10));

    // tft.print("Request RPM");
}

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // Inicializa display
    tft.init(240, 320);        // ✅ ajuste para 240x320
    tft.setRotation(5);           // ✅ rotação correta (corrige espelhamento)
    tft.invertDisplay(false);
    tft.fillScreen(ST77XX_BLACK);
    tft.print("Iniciaizando");
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(20, 40);
    tft.setTextSize(3);
    tft.print("RPM: ");
    // tft.println(rpm);

    setupCAN();
}

void loop()
{
    requestRPM();
    // delay(10);

    twai_message_t message;

    if (twai_receive(&message, pdMS_TO_TICKS(10)) == ESP_OK)
    {
        // Respostas geralmente vêm de 0x7E8
        if (message.identifier >= 0x7E8 && message.identifier <= 0x7EF)
        {
            if (message.data[2] == 0x0C)
            {
                int A = message.data[3];
                int B = message.data[4];
                int rpm = ((A * 256) + B) / 4;

                Serial.print("RPM: ");
                Serial.println(rpm);

                // tft.fillScreen(ST77XX_BLACK);
                // tft.setCursor(20, 40);
                // tft.setTextSize(3);
                // tft.print("RPM: ");
                // tft.println(rpm);

                   tft.fillRect(120,40,100,30,ST77XX_BLACK);

                tft.setCursor(120,40);
                tft.print(rpm);
            }
        }
    }
}