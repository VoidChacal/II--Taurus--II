#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CE_PIN 4
#define CSN_PIN 5

#define NUM_CHANNELS 126  // Quantos canais mostrar (máx. 126)
#define SAMPLES 30       // Leituras por canal (menos = mais rápido)
#define BAR_WIDTH 1      // Largura de cada barra em pixels

RF24 radio(CE_PIN, CSN_PIN);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

uint8_t values[126];

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 falhou"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  if (!radio.begin()) {
    Serial.println(F("nRF24L01 não encontrado!"));
    while (1);
  }

  radio.setAutoAck(false);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void scanChannels() {
  for (uint8_t ch = 0; ch < NUM_CHANNELS; ch++) {
    int count = 0;
    for (int s = 0; s < SAMPLES; s++) {
      radio.setChannel(ch);
      radio.startListening();
      delayMicroseconds(150);
      if (radio.testRPD()) count++;
      radio.stopListening();
    }
    values[ch] = map(count, 0, SAMPLES, 0, 100);
  }
}


void drawSpectrum() {
  display.clearDisplay();
  for (uint8_t ch = 0; ch < NUM_CHANNELS; ch++) {
    int barHeight = map(values[ch], 0, 50, 0, SCREEN_HEIGHT);
    int x = ch * BAR_WIDTH;
    display.fillRect(x, SCREEN_HEIGHT - barHeight, BAR_WIDTH, barHeight, WHITE);
  }
  display.display();
}

void loop() {
  scanChannels();
  drawSpectrum();
  delay(50);
}
