#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CE_PIN  4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
Adafruit_SSD1306 display(128, 64, &Wire);

uint8_t values[126];

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED não encontrado!"));
    for(;;);
  }
  display.clearDisplay();
  display.display();

  if (!radio.begin()) {
    Serial.println(F("Falha ao iniciar nRF24L01"));
    for(;;);
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  // Faz a varredura
  for (int ch = 0; ch <= 125; ch++) {
    radio.setChannel(ch);
    radio.startListening();

    uint8_t count = 0;
    for (int i = 0; i < 100; i++) { // mais amostras = mais precisão
      //delayMicroseconds(120);
      if (radio.testRPD()) count++;
    }

    values[ch] = count;
    radio.stopListening();
  }

  // Mostra no OLED
  display.clearDisplay();
  for (int i = 0; i < 126 && i < 128; i++) {
    int height = map(values[i], 0, 100, 0, 63);
    display.drawLine(i, 63, i, 63 - height, WHITE);
  }
  display.display();
}
