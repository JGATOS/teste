#include <Arduino.h>
#include <driver/twai.h>
#include <esp_task_wdt.h>
#include "Interface.h"

#define CAN_TX 16
#define CAN_RX 15

volatile int currentRpm = 0;
volatile bool shiftLightAtivo = false;
bool wasShiftLightAtivo = false;

int currentEtanol = 0, currentAgua = 0, currentIat = 0;
float currentCombustivelBar = 0.0;
int lastEtanol = -1, lastAgua = -1, lastIat = -1;
float lastCombustivelBar = -1.0;

enum SEC_OBD_STATES
{
    REQ_ETANOL,
    REQ_AGUA,
    REQ_IAT,
    REQ_FUEL_PRESS
};
SEC_OBD_STATES secState = REQ_ETANOL;

unsigned long lastRpmRequestTime = 0;
unsigned long lastSecRequestTime = 0;
const unsigned long rpmRequestInterval = 12;
const unsigned long secRequestInterval = 80;

void setupCAN()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NORMAL);
    g_config.rx_queue_len = 100;
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

void sendOBDRequest(uint8_t pid)
{
    twai_message_t message;
    message.identifier = 0x7DF;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 8;
    message.data[0] = 0x02;
    message.data[1] = 0x01;
    message.data[2] = pid;
    for (int i = 3; i < 8; i++)
        message.data[i] = 0x55;
    twai_transmit(&message, pdMS_TO_TICKS(2));
}

void setup()
{
    Serial.begin(115200);
    iniciarDisplay();
    desenharInterfaceBase();
    setupCAN();
}

void loop()
{
    unsigned long currentMillis = millis();
    int msgsProcessadas = 0;
    twai_message_t rx_msg;

    while (twai_receive(&rx_msg, 0) == ESP_OK && msgsProcessadas < 25)
    {
        msgsProcessadas++;
        if (rx_msg.identifier == 0x7E8 && rx_msg.data[1] == 0x41)
        {
            uint8_t pid = rx_msg.data[2];
            if (pid == 0x0C)
            {
                currentRpm = ((rx_msg.data[3] * 256) + rx_msg.data[4]) / 4;
                shiftLightAtivo = (currentRpm >= SHIFT_LIGHT_RPM);
                desenharBarraRPM(currentRpm);
            }
            else if (pid == 0x52)
                currentEtanol = rx_msg.data[3] * (100.0 / 255.0);
            else if (pid == 0x05)
                currentAgua = rx_msg.data[3] - 40;
            else if (pid == 0x0F)
                currentIat = rx_msg.data[3] - 40;
            else if (pid == 0x23)
                currentCombustivelBar = (((rx_msg.data[3] * 256) + rx_msg.data[4])) / 10.0;
        }
    }

    if (currentMillis - lastRpmRequestTime >= rpmRequestInterval)
    {
        lastRpmRequestTime = currentMillis;
        sendOBDRequest(0x0C);
    }

    if (!shiftLightAtivo && (currentMillis - lastSecRequestTime >= secRequestInterval))
    {
        lastSecRequestTime = currentMillis;
        if (secState == REQ_ETANOL)
        {
            sendOBDRequest(0x52);
            secState = REQ_AGUA;
        }
        else if (secState == REQ_AGUA)
        {
            sendOBDRequest(0x05);
            secState = REQ_IAT;
        }
        else if (secState == REQ_IAT)
        {
            sendOBDRequest(0x0F);
            secState = REQ_FUEL_PRESS;
        }
        else
        {
            sendOBDRequest(0x23);
            secState = REQ_ETANOL;
        }
    }

    if (shiftLightAtivo)
    {
        wasShiftLightAtivo = true;
        if ((currentMillis / 100) % 2 == 0)
            desenharShiftLight();
        else
        {
            limparShiftLight();
            desenharBarraRPM(currentRpm);
        }
    }
    else
    {
        if (wasShiftLightAtivo)
        {
            limparShiftLight();
            desenharBarraRPM(currentRpm);
            lastEtanol = -1;
            lastAgua = -1;
            lastIat = -1;
            lastCombustivelBar = -1.0;
            wasShiftLightAtivo = false;
        }
        atualizarValores((int)currentEtanol, (int)currentAgua, (int)currentIat, (int)currentCombustivelBar,
                         lastEtanol, lastAgua, lastIat, lastCombustivelBar);
    }
}