#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>

// IMPORTANDO AS FONTES VETORIZADAS SUAVES
#include <Fonts/FreeSansBold24pt7b.h>  // Para textos Gigantes (SHIFT, UP!)
#include <Fonts/FreeSansBold18pt7b.h>  // Para os Valores numéricos
#include <Fonts/FreeSansBold12pt7b.h>  // Para Unidades (%, C, bar) e Menu

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

  // AQUI ESTÁ A MÁGICA DA ROTAÇÃO:
  // 0x28 = Paisagem Padrão. 0xE8 = Paisagem Invertida (Giro de 180 graus)
  uint8_t madctl_value = 0xE8;
  tft.sendCommand(0x36, &madctl_value, 1);

  tft.fillScreen(ST77XX_BLACK);

  ts.begin();
  // ts.setRotation(3);
}

void telaDeInicializacao() {
  tft.fillScreen(ST77XX_BLACK);

  // Texto Gigante vetorizado para "UP! TSI"
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextSize(1);  // Nunca escalamos fontes vetorizadas

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(60, 110);
  tft.print("UP!");

  tft.setTextColor(ST77XX_RED);
  tft.setCursor(160, 110);
  tft.print("TSI");

  // Texto Intermediário vetorizado
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 160);
  tft.print("RACING DASHBOARD");

  tft.drawRect(40, 190, 240, 12, ST77XX_WHITE);
  for (int i = 0; i <= 240; i += 15) {
    tft.fillRect(40, 190, i, 12, ST77XX_RED);
    delay(40);
  }
  delay(400);
}

void desenharLayoutBase() {
  tft.setFont(NULL);
  tft.fillScreen(ST77XX_BLACK);
  tft.drawFastHLine(0, 15, 320, ST77XX_WHITE);
  tft.drawFastHLine(0, 125, 320, ST77XX_WHITE);
  tft.drawFastVLine(160, 15, 225, ST77XX_WHITE);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(15, 25);
  tft.print("COMB / ETANOL");
  tft.setCursor(175, 25);
  tft.print("LIQ. ARREFECIMENTO");
  tft.setCursor(15, 135);
  tft.print("TEMP. ADMISSAO (IAT)");
  tft.setCursor(175, 135);
  tft.print("PRESSAO COMBUSTIVEL");

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
    tft.fillRect(12, 55, 135, 45, ST77XX_BLACK);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(15, 88);
    tft.print(etanol);

    int cursorX = tft.getCursorX();
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(cursorX + 4, 88);
    tft.print("%");
    lastEtanol = etanol;
  }

  if (agua != lastAgua) {
    tft.fillRect(172, 55, 135, 45, ST77XX_BLACK);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextSize(1);
    tft.setTextColor(agua > 105 ? ST77XX_RED : ST77XX_WHITE);
    tft.setCursor(175, 88);
    tft.print(agua);

    int cursorX = tft.getCursorX();
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(cursorX + 4, 88);
    tft.print("C");
    lastAgua = agua;
  }

  if (iat != lastIat) {
    tft.fillRect(12, 165, 135, 45, ST77XX_BLACK);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextSize(1);
    tft.setTextColor(iat > 50 ? ST77XX_RED : ST77XX_WHITE);
    tft.setCursor(15, 198);
    tft.print(iat);

    int cursorX = tft.getCursorX();
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(cursorX + 4, 198);
    tft.print("C");
    lastIat = iat;
  }

  if (abs(combustivelBar - lastCombustivelBar) > 0.05 || lastCombustivelBar < 0) {
    tft.fillRect(172, 165, 148, 45, ST77XX_BLACK);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextSize(1);
    tft.setTextColor(combustivelBar < 40.0 ? ST77XX_RED : ST77XX_WHITE);
    tft.setCursor(175, 198);
    tft.print(combustivelBar, 1);

    int cursorX = tft.getCursorX();
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(cursorX + 4, 198);
    tft.print("bar");
    lastCombustivelBar = combustivelBar;
  }
}

void desenharTelaConfig() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(30, 35);
  tft.print("AJUSTE SHIFT LIGHT");

  atualizarRpmConfig();

  // Botão Menos [-100]
  tft.drawRoundRect(20, 140, 110, 55, 6, ST77XX_WHITE);
  tft.setCursor(40, 178);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("-100");

  // Botão Mais [+100]
  tft.drawRoundRect(190, 140, 110, 55, 6, ST77XX_WHITE);
  tft.setCursor(205, 178);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("+100");

  // Botão Salvar
  tft.drawRoundRect(105, 80, 110, 40, 6, ST77XX_RED);
  tft.setCursor(118, 108);
  tft.setTextColor(ST77XX_RED);
  tft.print("SALVAR");
}

void atualizarRpmConfig() {
  tft.fillRect(60, 48, 200, 28, ST77XX_BLACK);

  tft.setFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(90, 72);
  tft.print(shiftLightRpm);

  int cursorX = tft.getCursorX();
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(cursorX + 8, 72);
  tft.print("RPM");
}

void desenharShiftLight() {
  tft.fillScreen(0xFD20);

  // SHIFT GIGANTE COM BORDAS SUAVES E ARREDONDADAS!
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(80, 135);
  tft.print("SHIFT");
}

void limparShiftLight() {
  desenharLayoutBase();
}