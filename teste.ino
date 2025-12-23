#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// CONFIGURE AQUI
const char* ssid = "HUAWEI-2.4G-sDNj";
const char* password = "t3PeGAY4X@";


// LCD I2C (ex: 0x27) 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);


// NRF24 (use os pinos corretos do seu modelo de ESP32)
RF24 radio(27, 14); // CE, CSN - ajuste conforme seu board
const byte address[6] = "00001"; // mesmo endereço do Arduino


WebServer server(80);


void handleRoot() {
String cmd = server.arg("cmd");
if (cmd.length() == 0) {
server.send(200, "text/plain", "Use /control?cmd=seu+comando");
return;
}


// envia via NRF24 para Arduino
radio.stopListening();
radio.write(cmd.c_str(), cmd.length() + 1);
radio.startListening();


// envia confirmação por Serial para PC
Serial.println(String("ESP_CMD:") + cmd);


server.send(200, "text/plain", "Comando enviado: " + cmd);
}

void setup() {
Serial.begin(115200);
delay(1000);


// LCD
lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ESP32 iniciando");


// Wifi
WiFi.begin(ssid, password);
lcd.setCursor(0,1);
lcd.print("Conectando...");
int attempts = 0;
while (WiFi.status() != WL_CONNECTED && attempts < 20) {
delay(500);
attempts++;
}
if (WiFi.status() == WL_CONNECTED) {
lcd.clear();
lcd.setCursor(0,0);
lcd.print("WiFi: OK");
lcd.setCursor(0,1);
lcd.print(WiFi.localIP().toString());
} else {
lcd.clear();
lcd.setCursor(0,0);
lcd.print("WiFi Falhou");
}


// NRF24
radio.begin();
radio.setPALevel(RF24_PA_LOW);
radio.openWritingPipe(address);
radio.openReadingPipe(0, address);
radio.startListening();


// Webserver endpoint
server.on("/control", handleRoot);
server.begin();
}


void loop() {
server.handleClient();


// Ler Serial vindo do PC (respostas do TAU) e encaminhar para Arduino
if (Serial.available()) {
String line = Serial.readStringUntil('\n');
if (line.length()) {
// limpar whitespace
line.trim();
// enviar via NRF
radio.stopListening();
radio.write(line.c_str(), line.length() + 1);
radio.startListening();


// update LCD status
lcd.clear();
lcd.setCursor(0,0);
lcd.print("TAU->Arduino:");
lcd.setCursor(0,1);
lcd.print(line.substring(0,16));
}
}
}