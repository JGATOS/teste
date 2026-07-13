#include <Arduino.h>
#include <driver/twai.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>
// AJUSTE: Incluída a biblioteca para gerenciar o Watchdog Timer (WDT)
#include <esp_task_wdt.h>

// Pinos do Display ST7789
#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   7
#define TFT_MOSI  35
#define TFT_SCK   36
#define TFT_BL    4

// Pinos do Touch XPT2046
#define TOUCH_CS    10    
#define TOUCH_IRQ   18    
#define TOUCH_MISO  37    

// Pinos do Transceiver CAN
#define CAN_TX 16
#define CAN_RX 15

#define SHIFT_LIGHT_RPM 6500

// Utiliza a instância SPI padrão do ESP32-S3
Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ); 

// Variáveis voláteis dos Sensores OBD2
volatile int currentRpm = 0;
volatile bool shiftLightAtivo = false;

int currentEtanol = 0, currentAgua = 0, currentIat = 0;
float currentCombustivelBar = 0.0;

// Variáveis de controle de tela
int lastEtanol = -1, lastAgua = -1, lastIat = -1;
float lastCombustivelBar = -1.0;
bool flashEstado = false; 
bool tcDesativadoEstado = false; 

// Gerenciamento da rajada TC OFF Assíncrona
bool enviandoRajadaTC = false;
int contadorFramesTC = 0;
unsigned long lastFrameTCTime = 0;

// Coordenadas do Botão Virtual Touch na Tela
const int btnX = 15;
const int btnY = 205;
const int btnW = 110;
const int btnH = 30;

enum SEC_OBD_STATES { REQ_ETANOL, REQ_AGUA, REQ_IAT, REQ_FUEL_PRESS };
SEC_OBD_STATES secState = REQ_ETANOL;

unsigned long lastRpmRequestTime = 0;
unsigned long lastSecRequestTime = 0;
unsigned long lastTouchTime = 0;
unsigned long lastFlashTime = 0;

const unsigned long rpmRequestInterval = 12;  
const unsigned long secRequestInterval = 200; 
const unsigned long flashInterval = 40;    

// Variáveis de controle UDS do ABS
unsigned long lastUdsTime = 0;
int etapaUDS = 0; 

void setupCAN() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_NORMAL);
    g_config.rx_queue_len = 100; 
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

void verificarStatusCAN() {
    twai_status_info_t status_info;
    twai_get_status_info(&status_info);
    if (status_info.state == TWAI_STATE_BUS_OFF) {
        twai_initiate_recovery(); 
        vTaskDelay(pdMS_TO_TICKS(10));
        twai_start();             
    }
}

inline void sendOBDRequest(uint8_t pid) {
    twai_message_t message;
    message.identifier = 0x7DF;   
    message.extd = 0;             
    message.rtr = 0;
    message.data_length_code = 8;
    message.data[0] = 0x02; 
    message.data[1] = 0x01; 
    message.data[2] = pid;  
    for(int i=3; i<8; i++) message.data[i] = 0x55; 

    twai_transmit(&message, pdMS_TO_TICKS(2));
}

void desenharInterfaceBase(bool emAlerta) {
    tft.fillScreen(ST77XX_BLACK); 
    
    if (!emAlerta) {
        tft.setTextColor(ST77XX_WHITE);
        tft.setTextSize(2);
        tft.setCursor(15, 20); tft.print("ETANOL:");
        tft.setCursor(170, 20); tft.print("AGUA:");
        tft.setCursor(15, 120); tft.print("(IAT):");
        tft.setCursor(170, 120); tft.print("P. COMB:"); 
        
        lastEtanol = -1; lastAgua = -1; lastIat = -1; lastCombustivelBar = -1.0;
    }
}

void setup() {
    Serial.begin(115200);
    
    // ANTES DO AJUSTE: Não havia monitoramento de travamento (Watchdog).
    // DEPOIS DO AJUSTE: Inicializa o Watchdog com 10 segundos de timeout.
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = 10000, // 10 segundos
        .idle_core_mask = (1 << 0), // Monitorar o core 0
        .trigger_panic = true // Reiniciar em caso de timeout
    };
    esp_task_wdt_init(&wdt_config);
    esp_task_wdt_add(NULL); // Adiciona a tarefa atual ao monitoramento

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    SPI.begin(TFT_SCK, TOUCH_MISO, TFT_MOSI, -1);

    tft.init(240, 320);        
    tft.setRotation(5);    
    tft.invertDisplay(false); 
    
    ts.begin(SPI);
    ts.setRotation(1); 

    desenharInterfaceBase(false);
    setupCAN();
}

void loop() {
    // ANTES DO AJUSTE: O processador poderia travar silenciosamente.
    // DEPOIS DO AJUSTE: Reseta o timer do Watchdog a cada ciclo do loop.
    esp_task_wdt_reset(); 

    unsigned long currentMillis = millis();
    
    verificarStatusCAN();

    if (currentMillis - lastTouchTime > 200) { 
        if (ts.touched()) { 
            TS_Point p = ts.getPoint();
            int xPixel = map(p.x, 300, 3800, 0, 320);
            int yPixel = map(p.y, 300, 3800, 0, 240);

            if (xPixel >= btnX && xPixel <= (btnX + btnW) && yPixel >= btnY && yPixel <= (btnY + btnH)) {
                lastTouchTime = currentMillis;
                if (!shiftLightAtivo) {
                    tcDesativadoEstado = !tcDesativadoEstado;
                }
            }
        }
    }

    if (currentMillis - lastRpmRequestTime >= rpmRequestInterval) {
        lastRpmRequestTime = currentMillis;
        sendOBDRequest(0x0C);
    }

    if (!shiftLightAtivo && (currentMillis - lastSecRequestTime >= secRequestInterval)) {
        lastSecRequestTime = currentMillis;
        switch (secState) {
            case REQ_ETANOL:     sendOBDRequest(0x52); secState = REQ_AGUA; break;
            case REQ_AGUA:       sendOBDRequest(0x05); secState = REQ_IAT; break;
            case REQ_IAT:        sendOBDRequest(0x0F); secState = REQ_FUEL_PRESS; break;
            case REQ_FUEL_PRESS: sendOBDRequest(0x23); secState = REQ_ETANOL; break;
        }
    }

    twai_message_t rx_msg;
    while (twai_receive(&rx_msg, 0) == ESP_OK) {
        if (rx_msg.identifier == 0x7E8 && rx_msg.data[0] >= 0x02 && rx_msg.data[1] == 0x41) {
            uint8_t pidRespondido = rx_msg.data[2];
            if (pidRespondido == 0x0C) { 
                currentRpm = ((rx_msg.data[3] * 256) + rx_msg.data[4]) / 4;
                if (currentRpm >= SHIFT_LIGHT_RPM) {
                    if (!shiftLightAtivo) shiftLightAtivo = true;
                } else {
                    if (shiftLightAtivo) {
                        shiftLightAtivo = false;
                        desenharInterfaceBase(false); 
                    }
                }
            } 
            else if (!shiftLightAtivo) { 
                switch (pidRespondido) {
                    case 0x52: currentEtanol = rx_msg.data[3] * (100.0 / 255.0); break;
                    case 0x05: currentAgua = rx_msg.data[3] - 40; break;
                    case 0x0F: currentIat = rx_msg.data[3] - 40; break;
                    case 0x23: currentCombustivelBar = (((rx_msg.data[3] * 256) + rx_msg.data[4])) / 100.0; break; 
                }
            }
        }
    }

    if (shiftLightAtivo) {
        if (currentMillis - lastFlashTime >= flashInterval) {
            lastFlashTime = currentMillis;
            flashEstado = !flashEstado;
            if (flashEstado) {
                tft.fillRect(5, 5, 310, 195, ST77XX_YELLOW); 
                tft.setTextColor(ST77XX_BLACK);
                tft.setTextSize(5);
                tft.setCursor(85, 80);
                tft.print("SHIFT");
            } else {
                tft.fillRect(5, 5, 310, 195, ST77XX_BLACK);
            }
        }
    } 
    else {
        tft.setTextSize(3);
        if (currentEtanol != lastEtanol) {
            tft.fillRect(15, 50, 110, 30, ST77XX_BLACK);
            tft.setCursor(15, 50); tft.setTextColor(ST77XX_GREEN);
            tft.print(currentEtanol); tft.print("%"); 
            lastEtanol = currentEtanol;
        }
        if (currentAgua != lastAgua) {
            tft.fillRect(170, 50, 110, 30, ST77XX_BLACK);
            tft.setCursor(170, 50);
            tft.setTextColor(currentAgua > 100 ? ST77XX_RED : ST77XX_CYAN);
            tft.print(currentAgua); tft.print("C"); 
            lastAgua = currentAgua;
        }
        if (currentIat != lastIat) {
            tft.fillRect(15, 150, 110, 30, ST77XX_BLACK);
            tft.setCursor(15, 150);
            tft.setTextColor(currentIat > 50 ? ST77XX_ORANGE : ST77XX_BLUE);
            tft.print(currentIat); tft.print("C"); 
            lastIat = currentIat;
        }
        if (currentCombustivelBar != lastCombustivelBar) {
            tft.fillRect(170, 150, 130, 30, ST77XX_BLACK);
            tft.setCursor(170, 150);
            tft.setTextColor(currentCombustivelBar < 3.0 ? ST77XX_RED : ST77XX_WHITE);
            tft.print(currentCombustivelBar, 1); tft.print("b"); 
            lastCombustivelBar = currentCombustivelBar;
        }
    }
}