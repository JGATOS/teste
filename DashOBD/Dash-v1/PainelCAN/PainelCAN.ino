#include <Arduino.h>
#include <driver/twai.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <XPT2046_Touchscreen.h>
#include <esp_task_wdt.h>
#include <Preferences.h>
#include "Interface.h"

// -------------------------------------------------------------
// PINO DO SD CARD - VERIFIQUE O PINO DA SUA PLACA
#define SD_CS 11 
// -------------------------------------------------------------

#define CAN_TX 16
#define CAN_RX 15

enum AppState { TELA_PRINCIPAL, TELA_MENU, TELA_CONFIG };
AppState currentState = TELA_PRINCIPAL;

// Variáveis Globais 
volatile int currentRpm = 0;
volatile bool shiftLightAtivo = false;
bool wasShiftLightAtivo = false;

// Controle de Log 
bool isLogging = false;
bool logBlinkState = false;
unsigned long lastLogBlink = 0;
unsigned long lastLogWrite = 0;
File logFile;

// Controle Touch, Timeout e Trava de Transição
unsigned long tempoInicioToque = 0;
unsigned long tempoDebounceToque = 0;
unsigned long tempoUltimoToque = 0;
bool esperandoLiberarToque = false; // <-- Nova trava de segurança para o touch

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
  message.data[0] = 0x02;
  message.data[1] = 0x01;
  message.data[2] = pid;
  for (int i = 3; i < 8; i++) message.data[i] = 0x55;
  twai_transmit(&message, pdMS_TO_TICKS(2));
}

void setup() {
  Serial.begin(115200);

  preferences.begin("shift_light", false);
  shiftLightRpm = preferences.getInt("rpm_salvo", 6500);

  iniciarDisplay();
  telaDeInicializacao();
  
  if (!SD.begin(SD_CS)) {
    Serial.println("ERRO: Falha ao inicializar o SD Card!");
  }

  desenharLayoutBase();
  setupCAN();
}

void toggleLog() {
  if (!isLogging) {
    logFile = SD.open("/datalog.csv", FILE_APPEND);
    if (logFile) {
      isLogging = true;
      if (logFile.size() == 0) { 
        // Alterado para Ponto e Vírgula (;)
        logFile.println("Tempo(ms);RPM;Etanol(%);Agua(C);IAT(C);Pressao_Combustivel(bar)");
        logFile.flush(); // Garante a gravação do cabeçalho
      }
    }
  } else {
    isLogging = false;
    if (logFile) {
      logFile.flush(); // Salva qualquer resto no buffer
      logFile.close();
    }
  }
}

void gravarLogRotina() {
  if (isLogging) {
    if (logFile) {
      logFile.print(millis()); logFile.print(";");
      logFile.print(currentRpm); logFile.print(";");
      logFile.print(currentEtanol); logFile.print(";");
      logFile.print(currentAgua); logFile.print(";");
      logFile.print(currentIat); logFile.print(";");
      logFile.println(currentCombustivelBar, 1);
      
      logFile.flush(); // <-- OBRIGA A SALVAR NO DISCO AGORA. Previne corromper se desligar.
    } else {
      isLogging = false; // Se o arquivo ficar inacessível (ex: SD balançou), para de tentar gravar para não travar o ESP32.
    }
  }
}

void voltarParaPrincipal() {
    currentState = TELA_PRINCIPAL;
    desenharLayoutBase();
    if (isLogging) desenharIndicadorLog(isLogging, logBlinkState);
    lastEtanol = -1; lastAgua = -1; lastIat = -1; lastCombustivelBar = -1.0;
}

void loop() {
  unsigned long currentMillis = millis();
  int msgsProcessadas = 0;
  twai_message_t rx_msg;

  // 1. LEITURA DE DADOS CAN
  while (twai_receive(&rx_msg, 0) == ESP_OK && msgsProcessadas < 25) {
    msgsProcessadas++;
    if (rx_msg.identifier == 0x7E8 && rx_msg.data[1] == 0x41) {
      uint8_t pid = rx_msg.data[2];
      if (pid == 0x0C) {
        currentRpm = ((rx_msg.data[3] * 256) + rx_msg.data[4]) / 4;
        if (currentRpm >= shiftLightRpm) shiftLightAtivo = true;
        else if (currentRpm < (shiftLightRpm - 100)) shiftLightAtivo = false;
        
        if (currentState == TELA_PRINCIPAL && !shiftLightAtivo) desenharBarraRPM(currentRpm);
      } 
      else if (pid == 0x52) currentEtanol = rx_msg.data[3] * (100.0 / 255.0);
      else if (pid == 0x05) currentAgua = rx_msg.data[3] - 40;
      else if (pid == 0x0F) currentIat = rx_msg.data[3] - 40;
      else if (pid == 0x23) currentCombustivelBar = ((rx_msg.data[3] * 256.0) + rx_msg.data[4]) * 0.1;
    }
  }

  // 2. REQUISITORES OBD-II
  if (currentMillis - lastRpmRequestTime >= rpmRequestInterval) {
    lastRpmRequestTime = currentMillis; sendOBDRequest(0x0C);
  }
  if (!shiftLightAtivo && currentState == TELA_PRINCIPAL && (currentMillis - lastSecRequestTime >= secRequestInterval)) {
    lastSecRequestTime = currentMillis;
    if (secState == REQ_ETANOL) { sendOBDRequest(0x52); secState = REQ_AGUA; }
    else if (secState == REQ_AGUA) { sendOBDRequest(0x05); secState = REQ_IAT; }
    else if (secState == REQ_IAT) { sendOBDRequest(0x0F); secState = REQ_FUEL_PRESS; }
    else if (secState == REQ_FUEL_PRESS) { sendOBDRequest(0x23); secState = REQ_ETANOL; }
  }

  // 3. GERENCIADOR DE DATALOGGER (GRAVAÇÃO E INDICADOR)
  if (isLogging) {
    if (currentMillis - lastLogWrite >= 200) { 
      lastLogWrite = currentMillis;
      gravarLogRotina(); 
    }
    if (currentMillis - lastLogBlink >= 500) { 
      lastLogBlink = currentMillis;
      logBlinkState = !logBlinkState;
      if (currentState == TELA_PRINCIPAL && !shiftLightAtivo) desenharIndicadorLog(isLogging, logBlinkState);
    }
  }

  // 4. GERENCIADOR DE TOQUE ANTIFALHAS
  if (ts.touched()) {
    if (esperandoLiberarToque) {
      // O usuário está segurando a tela após mudar de menu. Não faz nada até ele soltar.
      tempoInicioToque = 0; 
    } else {
      TS_Point p = ts.getPoint();
      int touchX = map(p.x, 3800, 200, 0, 320); 
      int touchY = map(p.y, 3800, 200, 0, 240); 
      
      touchX = constrain(touchX, 0, 320);
      touchY = constrain(touchY, 0, 240);

      // Se estiver na tela principal, aguarda toque longo (1,5s) para abrir o Menu
      if (currentState == TELA_PRINCIPAL) {
        if (tempoInicioToque == 0) {
          tempoInicioToque = currentMillis;
        } else if (currentMillis - tempoInicioToque >= 1500) { // Toque Longo de 1.5s
          currentState = TELA_MENU;
          desenharTelaOpcoes(isLogging);
          tempoInicioToque = 0;
          tempoDebounceToque = currentMillis;
          tempoUltimoToque = currentMillis;
          esperandoLiberarToque = true; // Trava os toques até soltar o dedo
        }
      } 
      // Trata toques nas telas de Menu e Configuração
      else {
        tempoInicioToque = 0; 
        if (currentMillis - tempoDebounceToque > 300) {
          tempoDebounceToque = currentMillis;
          tempoUltimoToque = currentMillis; // Reseta o timeout a cada interação

          if (currentState == TELA_MENU) {
            if (touchY >= 50 && touchY <= 115) { // Clicou Gravar/Parar LOG
              toggleLog();
              voltarParaPrincipal();
              esperandoLiberarToque = true;
            } 
            else if (touchY >= 120 && touchY <= 180) { // Clicou Shift Light
              currentState = TELA_CONFIG;
              desenharTelaConfig();
              esperandoLiberarToque = true;
            }
            else if (touchY >= 190 && touchY <= 240) { // Clicou Voltar
              voltarParaPrincipal();
              esperandoLiberarToque = true;
            }
          } 
          else if (currentState == TELA_CONFIG) {
            if (touchX >= 35 && touchX <= 115 && touchY >= 145 && touchY <= 190) {
              if (shiftLightRpm > 2000) shiftLightRpm -= 100;
              atualizarRpmConfig();
            } else if (touchX >= 205 && touchX <= 285 && touchY >= 145 && touchY <= 190) {
              if (shiftLightRpm < 9000) shiftLightRpm += 100;
              atualizarRpmConfig();
            }
          }
        }
      }
    }
  } else {
    // Dedo fora da tela: reseta os contadores e destrava o sistema
    tempoInicioToque = 0; 
    esperandoLiberarToque = false; 
  }

  // 5. TIMEOUTS: VOLTA PARA TELA PRINCIPAL APÓS 5 SEGS DE INATIVIDADE
  if (currentState == TELA_MENU && (currentMillis - tempoUltimoToque > 5000)) {
      voltarParaPrincipal();
  }
  
  if (currentState == TELA_CONFIG && (currentMillis - tempoUltimoToque > 5000)) {
      preferences.putInt("rpm_salvo", shiftLightRpm); // Salva antes de sair
      voltarParaPrincipal();
  }

  // 6. ATUALIZAÇÃO DO FRAME GRÁFICO (Tela Principal)
  if (currentState == TELA_PRINCIPAL) {
    if (shiftLightAtivo) {
      if (!wasShiftLightAtivo) {
        desenharShiftLight();
        wasShiftLightAtivo = true;
      }
    } else {
      if (wasShiftLightAtivo) {
        desenharLayoutBase();
        if (isLogging) desenharIndicadorLog(isLogging, logBlinkState);
        lastEtanol = -1; lastAgua = -1; lastIat = -1; lastCombustivelBar = -1.0;
        wasShiftLightAtivo = false;
      }
      desenharBarraRPM(currentRpm);
      atualizarValores(currentEtanol, currentAgua, currentIat, currentCombustivelBar,
                       lastEtanol, lastAgua, lastIat, lastCombustivelBar);
    }
  }
}