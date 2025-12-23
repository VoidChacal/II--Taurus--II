#include <TM1637Display.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>


// TM1637 pins
#define CLK 2
#define DIO 3
TM1637Display display(CLK, DIO);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// NRF24 (CE, CSN)
RF24 radio(9, 10);
const byte address[6] = "00001";


unsigned long startMillis;


void setup() {
Serial.begin(115200);
// TM1637
display.setBrightness(7);


// OLED
if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
// Falha ao iniciar
}
oled.clearDisplay();
oled.setTextSize(1);
oled.setTextColor(SSD1306_WHITE);
oled.setCursor(0,0);
oled.println("TAU-UNO iniciando...");
oled.display();


// NRF24
radio.begin();
radio.openReadingPipe(0, address);
radio.setPALevel(RF24_PA_LOW);
radio.startListening();


startMillis = millis();
}


void showUptime() {
unsigned long seconds = (millis() - startMillis) / 1000;
// formatar até 4 dígitos (0-9999)
if (seconds > 9999) seconds = 9999;
display.showNumberDec((int)seconds, true);
}


void showOLED(const char* msg) {
oled.clearDisplay();
oled.setCursor(0,0);
oled.println("TAU Resposta:");
oled.println(msg);
oled.display();
}


void loop() {
showUptime();


if (radio.available()) {
char buffer[64] = {0};
radio.read(&buffer, sizeof(buffer));
// buffer contém string '\0' terminated
showOLED(buffer);
// opcional: enviar ACK serial pro PC
Serial.print("NRF_RX:");
Serial.println(buffer);
}


// também escuta comandos via Serial (opcional: para debug)
if (Serial.available()) {
String line = Serial.readStringUntil('\n');
if (line.length() > 0) {
showOLED(line.c_str());
}
}


delay(200);
}