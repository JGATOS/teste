#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>

// ===================================================
// DEFINIÇÕES DE PINOS — AJUSTE CONFORME SUA LIGAÇÃO
// ===================================================
#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   7
#define TFT_MOSI  35
#define TFT_SCK   36

#define TOUCH_CS  10
#define TOUCH_IRQ 9

// ===================================================
// INSTÂNCIAS SPI / DISPLAY / TOUCH
// ===================================================
SPIClass spiBus(HSPI);
Adafruit_ST7789 tft(&spiBus, TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

// ===================================================
// SETUP
// ===================================================
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Iniciando ST7789 + XPT2046 (ESP32-S3)...");

  // Inicia o barramento SPI com os pinos definidos
  spiBus.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);

  // Inicializa display
  tft.init(240, 320);
  tft.setRotation(5); // ajuste de orientação
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Display OK");

  // Inicializa touch (usa o mesmo SPI)
  if (!ts.begin()) {
    Serial.println("Falha ao inicializar touch!");
    tft.setCursor(10, 40);
    tft.setTextColor(ST77XX_RED);
    tft.println("Touch FAIL");
  } else {
    Serial.println("Touch inicializado com sucesso!");
    tft.setCursor(10, 40);
    tft.setTextColor(ST77XX_GREEN);
    tft.println("Touch OK!");
  }

  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Toque na tela...");
}

// ===================================================
// LOOP PRINCIPAL
// ===================================================
void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    // --- Calibração (ajuste conforme seu módulo)
    int x = map(p.x, 200, 3800, 0, 239);
    int y = map(p.y, 200, 3800, 0, 319);
    x = constrain(x, 0, 239);
    y = constrain(y, 0, 319);

    Serial.printf("Toque detectado: rawX=%d rawY=%d -> X=%d Y=%d\n",
                  p.x, p.y, x, y);

    // Desenha um ponto amarelo no local do toque
    tft.fillCircle(x, y, 4, ST77XX_YELLOW);

    // Mostra coordenadas na parte inferior da tela
    tft.fillRect(0, 300, 240, 20, ST77XX_BLACK);
    tft.setCursor(10, 305);
    tft.setTextColor(ST77XX_CYAN);
    tft.printf("X:%03d  Y:%03d", x, y);

    delay(120);
  }
}
