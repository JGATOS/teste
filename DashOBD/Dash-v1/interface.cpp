#include "Interface.h"

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);

void iniciarDisplay()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    SPI.begin(TFT_SCK, -1, TFT_MOSI, -1); // MISO não usado pro display
    tft.init(240, 320);
    tft.setRotation(5);
    tft.invertDisplay(false);
    tft.fillScreen(ST77XX_BLACK);
}

void desenharInterfaceBase()
{
    tft.fillScreen(ST77XX_BLACK);

    // Desenha as bordas dos 4 "Cards" minimalistas
    tft.drawRoundRect(10, 25, 145, 95, 8, ST77XX_WHITE);   // Card Etanol
    tft.drawRoundRect(165, 25, 145, 95, 8, ST77XX_WHITE);  // Card Água
    tft.drawRoundRect(10, 130, 145, 95, 8, ST77XX_WHITE);  // Card IAT
    tft.drawRoundRect(165, 130, 145, 95, 8, ST77XX_WHITE); // Card Combustível

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);

    // Títulos centralizados nos cards
    tft.setCursor(45, 35);
    tft.print("ETANOL");
    tft.setCursor(210, 35);
    tft.print("AGUA");
    tft.setCursor(60, 140);
    tft.print("IAT");
    tft.setCursor(185, 140);
    tft.print("PRESSAO");
}

void desenharBarraRPM(int currentRpm)
{
    int larguraBarra = map(currentRpm, 0, SHIFT_LIGHT_RPM, 0, 320);
    larguraBarra = constrain(larguraBarra, 0, 320);

    uint16_t corBarra = ST77XX_GREEN;
    if (currentRpm > (SHIFT_LIGHT_RPM * 0.7))
        corBarra = ST77XX_YELLOW;
    if (currentRpm > (SHIFT_LIGHT_RPM * 0.9))
        corBarra = ST77XX_RED;

    tft.fillRect(0, 0, larguraBarra, 12, corBarra);
    if (larguraBarra < 320)
    {
        tft.fillRect(larguraBarra, 0, 320 - larguraBarra, 12, ST77XX_BLACK);
    }
}

void atualizarValores(int etanol, int agua, int iat, int combustivelBar,
                      int &lastEtanol, int &lastAgua, int &lastIat, float &lastCombustivelBar)
{
    tft.setTextSize(4);

    if (etanol != lastEtanol)
    {
        tft.fillRect(15, 65, 135, 40, ST77XX_BLACK);
        tft.setCursor(30, 70);
        tft.setTextColor(ST77XX_GREEN);
        tft.print(etanol);
        tft.setTextSize(2);
        tft.print("%");
        tft.setTextSize(4);
        lastEtanol = etanol;
    }
    if (agua != lastAgua)
    {
        tft.fillRect(170, 65, 135, 40, ST77XX_BLACK);
        tft.setCursor(190, 70);
        tft.setTextColor(agua > 100 ? ST77XX_RED : ST77XX_CYAN);
        tft.print(agua);
        tft.setTextSize(2);
        tft.print("C");
        tft.setTextSize(4);
        lastAgua = agua;
    }
    if (iat != lastIat)
    {
        tft.fillRect(15, 170, 135, 40, ST77XX_BLACK);
        tft.setCursor(30, 175);
        tft.setTextColor(iat > 50 ? ST77XX_ORANGE : ST77XX_BLUE);
        tft.print(iat);
        tft.setTextSize(2);
        tft.print("C");
        tft.setTextSize(4);
        lastIat = iat;
    }
    if (combustivelBar != (int)lastCombustivelBar)
    {
        tft.fillRect(170, 170, 135, 40, ST77XX_BLACK);
        tft.setCursor(190, 175);
        tft.setTextColor(combustivelBar < 30 ? ST77XX_RED : ST77XX_WHITE);
        tft.print(combustivelBar);
        tft.setTextSize(2);
        tft.print("b");
        tft.setTextSize(4);
        lastCombustivelBar = combustivelBar;
    }
}

void desenharShiftLight()
{
    tft.fillRect(0, 0, 320, 240, ST77XX_RED);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(6);
    tft.setCursor(65, 100);
    tft.print("SHIFT");
}

void limparShiftLight()
{
    desenharInterfaceBase();
}