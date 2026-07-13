#include <Arduino.h>
#include <driver/twai.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>
#include <esp_task_wdt.h>
#include <Preferences.h> // NOVO: Biblioteca para salvar dados permanentemente na Flash do ESP32
#include "Interface.h"

#define CAN_TX 16
#define CAN_RX 15

volatile int currentRpm = 0;
volatile bool shiftLightAtivo = false;
bool wasShiftLightAtivo = false; 

// Controle de Estado do Menu de Ajuste
bool emConfiguracao = false;
unsigned long tempoInicioToque = 0;   
unsigned long tempoDebounceToque = 0;
unsigned long tempoUltimoToque = 0;    // NOVO: Controla o timeout de 5 segundos para auto-salvamento

// Instancia o gerenciador de memória permanente
Preferences preferences;

int currentEtanol = 0, currentAgua = 0, currentIat = 0;
float currentCombustivelBar = 0.0;

int lastEtanol = -1, lastAgua = -1, lastIat = -1;
float lastCombustivelBar = -1.0;

enum SEC_OBD_STATES { REQ_ETANOL, REQ_AGUA, REQ_IAT, REQ_FUEL_PRESS };
SEC_OBD_STATES secState = REQ_ETANOL;

unsigned long lastRpmRequestTime = 0;
unsigned long lastSecRequestTime = 0;

const unsigned long rpmRequestInterval = 12;  
const unsigned long secRequestInterval = 80; 

void setupCAN() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NORMAL);
    g_config.rx_queue_len = 100; 
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

void sendOBDRequest(uint8_t pid) {
    twai_message_t message;
    message.identifier = 0x7DF;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = 8;
    message.data[0] = 0x02; message.data[1] = 0x01; message.data[2] = pid;
    for(int i=3; i<8; i++) message.data[i] = 0x55;
    twai_transmit(&message, pdMS_TO_TICKS(2));
}

void setup() {
    Serial.begin(115200);
    
    // NOVO: Inicializa a memória permanente e busca o RPM salvo. Se não houver, usa 6500.
    preferences.begin("shift_light", false);
    shiftLightRpm = preferences.getInt("rpm_salvo", 6500);
    
    iniciarDisplay();
    telaDeInicializacao();
    desenharLayoutBase();
    
    setupCAN();
}

void loop() {
    unsigned long currentMillis = millis();
    int msgsProcessadas = 0;
    twai_message_t rx_msg;
    
    // 1. LEITURA DE DADOS DA REDE CAN
    while (twai_receive(&rx_msg, 0) == ESP_OK && msgsProcessadas < 25) {
        msgsProcessadas++;
        if (rx_msg.identifier == 0x7E8 && rx_msg.data[1] == 0x41) {
            uint8_t pid = rx_msg.data[2];
            if (pid == 0x0C) { 
                currentRpm = ((rx_msg.data[3] * 256) + rx_msg.data[4]) / 4; 
                shiftLightAtivo = (currentRpm >= shiftLightRpm); 
                if (!emConfiguracao) desenharBarraRPM(currentRpm); 
            }
            else if (pid == 0x52) currentEtanol = rx_msg.data[3] * (100.0 / 255.0);
            else if (pid == 0x05) currentAgua = rx_msg.data[3] - 40;
            else if (pid == 0x0F) currentIat = rx_msg.data[3] - 40;
            else if (pid == 0x23) currentCombustivelBar = ((rx_msg.data[3] * 256) + rx_msg.data[4]);
        }
    }

    // 2. REQUISITORES OBD-II
    if (currentMillis - lastRpmRequestTime >= rpmRequestInterval) { 
        lastRpmRequestTime = currentMillis; sendOBDRequest(0x0C); 
    }
    if (!shiftLightAtivo && !emConfiguracao && (currentMillis - lastSecRequestTime >= secRequestInterval)) {
        lastSecRequestTime = currentMillis;
        if (secState == REQ_ETANOL) { sendOBDRequest(0x52); secState = REQ_AGUA; }
        else if (secState == REQ_AGUA) { sendOBDRequest(0x05); secState = REQ_IAT; }
        else if (secState == REQ_IAT) { sendOBDRequest(0x0F); secState = REQ_FUEL_PRESS; }
        else if (secState == REQ_FUEL_PRESS) { sendOBDRequest(0x23); secState = REQ_ETANOL; }
    }

    // 3. GERENCIADOR DE TOQUE ANTIFALHAS
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        int touchX = map(p.x, 3800, 200, 0, 320);
        int touchY = map(p.y, 200, 3800, 0, 240);
        touchX = constrain(touchX, 0, 320);
        touchY = constrain(touchY, 0, 240);

        if (!emConfiguracao) {
            // Long Press de 1.5 segundos para entrar na tela de configuração
            if (tempoInicioToque == 0) {
                tempoInicioToque = currentMillis; 
            } else if (currentMillis - tempoInicioToque >= 1500) {
                emConfiguracao = true;
                desenharTelaConfig();
                tempoInicioToque = 0;
                tempoDebounceToque = currentMillis; 
                tempoUltimoToque = currentMillis; // Dispara a contagem dos 5s de inatividade
            }
        } else {
            // Dentro do menu de ajuste
            if (currentMillis - tempoDebounceToque > 250) {
                tempoDebounceToque = currentMillis;
                
                // Botão Menos [-100] (Hitbox restrita)
                if (touchX >= 35 && touchX <= 115 && touchY >= 145 && touchY <= 190) {
                    if (shiftLightRpm > 2000) shiftLightRpm -= 100;
                    atualizarRpmConfig();
                    tempoUltimoToque = currentMillis; // Reseta os 5 segundos pois você interagiu
                }
                // Botão Mais [+100] (Hitbox restrita)
                else if (touchX >= 205 && touchX <= 285 && touchY >= 145 && touchY <= 190) {
                    if (shiftLightRpm < 9000) shiftLightRpm += 100;
                    atualizarRpmConfig();
                    tempoUltimoToque = currentMillis; // Reseta os 5 segundos
                }
            }
        }
    } else {
        tempoInicioToque = 0; 
    }

    // NOVO: Lógica de Auto-salvamento por inatividade (5 segundos sem tocar nos botões)
    if (emConfiguracao && (currentMillis - tempoUltimoToque > 5000)) {
        preferences.putInt("rpm_salvo", shiftLightRpm); // Grava o valor definitivo na Flash
        emConfiguracao = false;
        desenharLayoutBase();
        
        // Força a atualização completa dos gráficos na volta ao painel
        lastEtanol = -1; lastAgua = -1; lastIat = -1; lastCombustivelBar = -1.0;
    }

    // 4. ATUALIZAÇÃO DO FRAME GRÁFICO
    if (!emConfiguracao) {
        if (shiftLightAtivo) {
            wasShiftLightAtivo = true; 
            if ((currentMillis / 100) % 2 == 0) desenharShiftLight();
            else { desenharLayoutBase(); desenharBarraRPM(currentRpm); }
        } else {
            if (wasShiftLightAtivo) {
                desenharLayoutBase();
                desenharBarraRPM(currentRpm);
                lastEtanol = -1; lastAgua = -1; lastIat = -1; lastCombustivelBar = -1.0;
                wasShiftLightAtivo = false;
            }
            atualizarValores(currentEtanol, currentAgua, currentIat, currentCombustivelBar, 
                             lastEtanol, lastAgua, lastIat, lastCombustivelBar);
        }
    }
}