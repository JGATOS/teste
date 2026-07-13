#ifndef INTERFACE_H
#define INTERFACE_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>

// Pinos do Display e Touchscreen
#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   7
#define TFT_MOSI  35
#define TFT_SCK   36
#define TFT_BL    4

#define TOUCH_CS    10    
#define TOUCH_IRQ   18    
#define TOUCH_MISO  37    

// Agora o RPM do Shift Light é uma variável ajustável
extern volatile int shiftLightRpm;

extern Adafruit_ST7789 tft;
extern XPT2046_Touchscreen ts;

// Funções da Interface
void iniciarDisplay();
void telaDeInicializacao();
void desenharLayoutBase();
void desenharBarraRPM(int currentRpm);
void atualizarValores(int etanol, int agua, int iat, float combustivelBar, 
                      int &lastEtanol, int &lastAgua, int &lastIat, float &lastCombustivelBar);
void desenharShiftLight();
void limparShiftLight();

// Novas Funções para o Menu de Configuração
void desenharTelaConfig();
void atualizarRpmConfig();

#endif