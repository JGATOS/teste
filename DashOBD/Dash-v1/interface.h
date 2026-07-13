#ifndef INTERFACE_H
#define INTERFACE_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Pinos do Display
#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   7
#define TFT_MOSI  35
#define TFT_SCK   36
#define TFT_BL    4

#define SHIFT_LIGHT_RPM 6500

extern Adafruit_ST7789 tft;

// Funções da interface
void iniciarDisplay();
void desenharInterfaceBase();
void desenharBarraRPM(int currentRpm);
void atualizarValores(int etanol, int agua, int iat, int combustivelBar, 
                      int &lastEtanol, int &lastAgua, int &lastIat, float &lastCombustivelBar);
void desenharShiftLight();
void limparShiftLight();

#endif