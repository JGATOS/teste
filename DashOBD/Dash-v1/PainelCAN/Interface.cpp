#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>
#include "Interface.h"

// Inicialização das variáveis globais
volatile int shiftLightRpm = 6500; 

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ); 

void iniciarDisplay() {
    pinMode(TFT_BL, OUTPUT); 
    digitalWrite(TFT_BL, HIGH);
    SPI.begin(TFT_SCK, TOUCH_MISO, TFT_MOSI, -1);
    
    tft.init(240, 320); 
    tft.setRotation(5); 
    tft.invertDisplay(false);
    
    // =========================================================================
    // AJUSTE CORRETO: 0x28 remove o espelhamento horizontal e mantém a barra no topo
    // =========================================================================
    uint8_t madctl_value = 0x28; 
    tft.sendCommand(0x36, &madctl_value, 1);
    
    tft.fillScreen(ST77XX_BLACK);
    
    ts.begin();
    ts.setRotation(3); 
}

void telaDeInicializacao() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(5);
    tft.setCursor(65, 80);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("UP!");
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(160, 80);
    tft.print("TSI");

    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(50, 140);
    tft.print("RACING DASHBOARD");

    tft.drawRect(40, 190, 240, 12, ST77XX_WHITE);
    for(int i = 0; i <= 240; i += 15) {
        tft.fillRect(40, 190, i, 12, ST77XX_RED);
        delay(40);
    }
    delay(400); 
}

void desenharLayoutBase() {
    tft.fillScreen(ST77XX_BLACK);
    tft.drawFastHLine(0, 15, 320, ST77XX_WHITE);
    tft.drawFastHLine(0, 125, 320, ST77XX_WHITE);
    tft.drawFastVLine(160, 15, 225, ST77XX_WHITE);
    
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.setCursor(15, 25);  tft.print("COMB / ETANOL");
    tft.setCursor(175, 25); tft.print("LIQ. ARREFECIMENTO");
    tft.setCursor(15, 135); tft.print("TEMP. ADMISSAO (IAT)");
    tft.setCursor(175, 135);tft.print("PRESSAO COMBUSTIVEL");
    
    tft.fillRect(15, 37, 20, 2, ST77XX_RED);
    tft.fillRect(175, 37, 20, 2, ST77XX_RED);
    tft.fillRect(15, 147, 20, 2, ST77XX_RED);
    tft.fillRect(175, 147, 20, 2, ST77XX_RED);
}

void desenharBarraRPM(int currentRpm) {
    int larguraBarra = map(currentRpm, 0, shiftLightRpm, 0, 316);
    larguraBarra = constrain(larguraBarra, 0, 316);
    
    tft.drawRect(0, 0, 320, 12, ST77XX_WHITE);
    uint16_t corBarra = ST77XX_WHITE; 
    if (currentRpm > (shiftLightRpm * 0.9)) corBarra = ST77XX_RED;
    
    if (larguraBarra > 0) tft.fillRect(2, 2, larguraBarra, 8, corBarra);
    if (larguraBarra < 316) tft.fillRect(2 + larguraBarra, 2, 316 - larguraBarra, 8, ST77XX_BLACK);
}

void atualizarValores(int etanol, int agua, int iat, float combustivelBar, 
                      int &lastEtanol, int &lastAgua, int &lastIat, float &lastCombustivelBar) {
    if (etanol != lastEtanol) { 
        tft.fillRect(12, 55, 135, 45, ST77XX_BLACK); tft.setCursor(15, 60); tft.setTextSize(4);
        tft.setTextColor(ST77XX_WHITE); tft.print(etanol); tft.setTextSize(2);
        tft.setTextColor(ST77XX_RED); tft.print(" %"); lastEtanol = etanol; 
    }
    if (agua != lastAgua) { 
        tft.fillRect(172, 55, 135, 45, ST77XX_BLACK); tft.setCursor(175, 60); tft.setTextSize(4); 
        tft.setTextColor(agua > 105 ? ST77XX_RED : ST77XX_WHITE); tft.print(agua); tft.setTextSize(2);
        tft.setTextColor(ST77XX_RED); tft.print(" C"); lastAgua = agua; 
    }
    if (iat != lastIat) { 
        tft.fillRect(12, 165, 135, 45, ST77XX_BLACK); tft.setCursor(15, 170); tft.setTextSize(4); 
        tft.setTextColor(iat > 50 ? ST77XX_RED : ST77XX_WHITE); tft.print(iat); tft.setTextSize(2);
        tft.setTextColor(ST77XX_RED); tft.print(" C"); lastIat = iat; 
    }
    // CORREÇÃO: Exibição sem casas decimais (,0) e alerta vermelho configurado para menos de 30 bar
    if (combustivelBar != lastCombustivelBar) { 
        tft.fillRect(172, 165, 135, 45, ST77XX_BLACK); tft.setCursor(175, 170); tft.setTextSize(4); 
        tft.setTextColor(combustivelBar < 30.0 ? ST77XX_RED : ST77XX_WHITE); tft.print(combustivelBar, 0); tft.setTextSize(2);
        tft.setTextColor(ST77XX_RED); tft.print(" bar"); lastCombustivelBar = combustivelBar; 
    }
}

void desenharTelaConfig() {
    tft.fillScreen(ST77XX_BLACK);
    
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(55, 20);
    tft.print("AJUSTE SHIFT LIGHT");
    
    atualizarRpmConfig();
    
    // Botão Menos [-100]
    tft.drawRoundRect(20, 140, 110, 55, 6, ST77XX_WHITE);
    tft.setCursor(45, 158); tft.setTextColor(ST77XX_WHITE); tft.print("-100");
    
    // Botão Mais [+100]
    tft.drawRoundRect(190, 140, 110, 55, 6, ST77XX_WHITE);
    tft.setCursor(215, 158); tft.setTextColor(ST77XX_WHITE); tft.print("+100");
    
    // Botão Salvar
    tft.drawRoundRect(105, 80, 110, 40, 6, ST77XX_RED);
    tft.setTextSize(2); tft.setCursor(125, 92); tft.setTextColor(ST77XX_RED); tft.print("SALVAR");
}

void atualizarRpmConfig() {
    tft.fillRect(60, 48, 200, 28, ST77XX_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(90, 50);
    tft.print(shiftLightRpm);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_RED);
    tft.print(" RPM");
}

void desenharShiftLight() {
    tft.fillRect(0, 0, 320, 240, ST77XX_RED);
    tft.setTextColor(ST77XX_WHITE); tft.setTextSize(5);
    tft.setCursor(85, 100); tft.print("SHIFT");
}

void limpiarShiftLight() {
    desenharLayoutBase();
}