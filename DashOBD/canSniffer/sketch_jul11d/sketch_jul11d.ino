#include <Arduino.h>
#include <driver/twai.h>

#define CAN_TX 16
#define CAN_RX 15

unsigned long lastHeartbeat = 0;

void setupCAN_Sniffer() {
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, TWAI_MODE_LISTEN_ONLY);
    g_config.rx_queue_len = 500; 
    
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); // O Up! usa 500kbps na PT-CAN
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        Serial.println("Driver CAN instalado no modo LISTEN ONLY.");
    } else {
        Serial.println("ERRO: Falha ao instalar o driver CAN!");
    }
    
    if (twai_start() == ESP_OK) {
        Serial.println("Sniffer CAN Iniciado. Aguardando pacotes...");
    } else {
        Serial.println("ERRO: Falha ao iniciar o CAN!");
    }
}

void setup() {
    Serial.begin(115200);
    // Tempo extra para o USB do ESP32-S3 ser reconhecido pelo Windows/Mac
    delay(3000); 
    
    Serial.println("\n\n=== INICIANDO SISTEMA ===");
    setupCAN_Sniffer();
    
    Serial.println("=== LOG DE TRAFEGO CAN ===");
    Serial.println("ID (HEX) | DLC | D0 D1 D2 D3 D4 D5 D6 D7");
}

void loop() {
    twai_message_t rx_msg;
    
    // Heartbeat: Imprime um ponto a cada 1 segundo para sabermos que não travou
    if (millis() - lastHeartbeat > 1000) {
        Serial.print(".");
        lastHeartbeat = millis();
    }
    
    if (twai_receive(&rx_msg, pdMS_TO_TICKS(1)) == ESP_OK) {
        
        // Vamos olhar o tráfego do ABS (713) e respostas (77D), além do Gateway genérico
        if (rx_msg.identifier >= 0x700 && rx_msg.identifier <= 0x7FF) {
            
            // Pula uma linha se estava imprimindo pontos do heartbeat
            Serial.println(); 
            
            Serial.printf("%03X      |  %d  | ", rx_msg.identifier, rx_msg.data_length_code);
            for (int i = 0; i < rx_msg.data_length_code; i++) {
                Serial.printf("%02X ", rx_msg.data[i]);
            }
            Serial.println();
        }
    }
}