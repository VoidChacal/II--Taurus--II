#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); // Nome Bluetooth visível
  Serial.println("Bluetooth pronto para emparelhar");
}

void loop() {
  if (SerialBT.available()) {
    char received = SerialBT.read();
    Serial.print("Recebido via Bluetooth: ");
    Serial.println(received);
  }

  if (Serial.available()) {
    SerialBT.write(Serial.read()); // Envia do monitor serial para o Bluetooth
  }
}
