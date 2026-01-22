#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>

// #define USE_ILI9341

// ===================================================
// DEFINIÇÕES DE PINOS DO DISPLAY ST7789
// ===================================================
#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   7
#define TFT_MOSI  35
#define TFT_SCK   36
// #define TFT_MISO  37
// ST7789 não usa MISO

// ===================================================
// DEFINIÇÕES DE PINOS DO TOUCH XPT2046
// ===================================================
#define TOUCH_IRQ   9     // TIRQ
#define TOUCH_MISO  37    // TDO
#define TOUCH_MOSI  35    // TDIN (compartilha com o display)
#define TOUCH_CS    10    // TCS
#define TOUCH_SCK   36    // TCLK (compartilha com o display)

// ===================================================
// INSTÂNCIAS
// ===================================================
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

// ===================================================
// SETUP
// ===================================================
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Iniciando teste ST7789 (240x320) + Touch...");

  // Inicializa display
  tft.init(240, 320);        // ✅ ajuste para 240x320
  tft.setRotation(5);           // ✅ rotação correta (corrige espelhamento)
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);

  // Inicializa touch
  if (!ts.begin()) {
    Serial.println("Falha ao inicializar touch!");
  } else {
    Serial.println("Touch inicializado com sucesso!");
  }

  // Teste RGB rápido
  tft.fillScreen(ST77XX_RED); delay(300);
  tft.fillScreen(ST77XX_GREEN); delay(300);
  tft.fillScreen(ST77XX_BLUE); delay(300);
  tft.fillScreen(ST77XX_BLACK); delay(300);

  // Texto inicial
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Teste Touch XPT2046");
  tft.setTextSize(1);
  tft.setCursor(10, 35);
  tft.println("Toque na tela...");
}

// ===================================================
// LOOP PRINCIPAL
// ===================================================
void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    int x = map(p.x, 200, 3800, 0, 239);  // eixo X calibrado
    int y = map(p.y, 200, 3800, 0, 319);  // eixo Y calibrado
    x = constrain(x, 0, 239);
    y = constrain(y, 0, 319);


    // Apenas mostra os valores brutos
    Serial.print("Toque: rawX=");
    Serial.print(p.x);
    Serial.print(" rawY=");
    Serial.println(p.y);

    // Mostra na tela
    tft.fillRect(0, 280, 240, 40, ST77XX_BLACK); // limpa área inferior
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(10, 290);
    tft.printf("X:%04d  Y:%04d", p.x, p.y);

    // Desenha ponto amarelo no toque
    // Aqui você pode mapear se quiser, mas por enquanto deixa como brutos
    tft.fillCircle(p.x / 16, p.y / 13, 3, ST77XX_YELLOW); // ajuste aproximado

    delay(80);
  }
}

