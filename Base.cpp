// JOYSTICK
#define pinVRx A2
#define pinVRy A1
#define pinSW 5

// LED & RGB
#define ledYellow 3
#define ledRed 4
#define ledGreen 7

#define PinRed 13
#define PinGreen 8
#define PinBlue 12

// LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  // ---------- LED & RGB Setup -----------
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  pinMode(PinRed, OUTPUT);
  pinMode(PinGreen, OUTPUT);
  pinMode(PinBlue, OUTPUT);

  // ---------- JoyStick Setup -----------
  pinMode(pinVRx, INPUT);
  pinMode(pinVRy, INPUT);
  pinMode(pinSW, INPUT);

  // ---------- LCD Setup ----------
  lcd.init();
  lcd.backlight();  // Uncomment to turn on LCD backlight if necessary
}

void loop() {
  // ----------- JoyStick Configuration ------------
  int valorVRx = analogRead(pinVRx);
  int valorVRy = analogRead(pinVRy);
  bool statusSW = digitalRead(pinSW);

  // Check JoyStick
  Serial.print("Valor VRx: ");
  Serial.print(valorVRx);
  Serial.print(" Valor VRy: ");
  Serial.print(valorVRy);
  Serial.print(" Botao: ");
  Serial.println(statusSW ? "Apertado" : "Solto");

  // Check LED
  if(statusSW){
    digitalWrite(ledYellow, HIGH);
  }else{
    digitalWrite(ledYellow, LOW);
  }

  if(valorVRx > 1020){
    digitalWrite(ledRed, HIGH);
  }else{
    digitalWrite(ledRed, LOW);
  }

  if(valorVRy > 1020){
    digitalWrite(ledGreen, HIGH);
  }else{
    digitalWrite(ledGreen, LOW);
  }
  
  // check RGB
  if(valorVRx == 0){
    digitalWrite(PinRed, HIGH);
    digitalWrite(PinGreen, HIGH);
  }
  else{
    digitalWrite(PinRed, LOW);
    digitalWrite(PinGreen, LOW);
  }
  if(valorVRy == 0){
    digitalWrite(PinGreen, HIGH);
    digitalWrite(PinBlue, HIGH);
  }
  else{
    digitalWrite(PinGreen, LOW);
    digitalWrite(PinBlue, LOW);
  }
}
