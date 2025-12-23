#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuração do OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pino do microfone KY-037 (saída analógica A0)
#define MIC_PIN 25 

void setup() {
  Serial.begin(115200);

  // Inicializa o OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Erro ao iniciar SSD1306"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Visualizador de Som");
  display.display();
  delay(1000);
}

void loop() {
  // Lê o microfone (valor de 0 a 4095 no ADC do ESP32)
  int micValue = analogRead(MIC_PIN);

  // Normaliza para altura da tela
  int barHeight = map(micValue, 0, 4095, 0, SCREEN_HEIGHT);

  display.clearDisplay();

  display.fillRect(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT - barHeight, 20, barHeight, SSD1306_WHITE);

  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("Mic: ");
  display.println(micValue);

  display.display();

  delay(30); 
}
