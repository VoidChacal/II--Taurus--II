#include <Wire.h>
#include <Servo.h>

// ----------------- Entradas -----------------
#define LedBlue 7
#define echoPin 13
#define trigPin 12

Servo myservo;

// ------------------ Entidades ------------------
long duration;
int distance;
int pos = 0;  
bool servoParado = false; 

// ------------------ DistanciaCalculation ------------------
int calculationDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  pinMode(LedBlue, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(11);
  Serial.begin(9600);
  myservo.write(pos);
}

void loop() {
  calculationDist(); 
  Serial.print("Distancia: ");
  Serial.println(distance);

  if (distance < 40) {
    digitalWrite(LedBlue, HIGH); 
    servoParado = true;          // Define o estado do servo como parado
    return;                      // Sai do loop atual 
  } else {
    digitalWrite(LedBlue, LOW);  
    servoParado = false;  
  }

  if (!servoParado) {
    for (; pos <= 180; pos++) { // Continua a partir da última posição
      myservo.write(pos);
      delay(15);
      calculationDist();
      Serial.print(pos);
      Serial.print(",");
      Serial.print(distance);
      Serial.print(".");

      if (distance < 50) {
        servoParado = true; 
        break;            
      }
    }

    for (; pos >= 0; pos--) { // Continua a partir da última posição
      myservo.write(pos);
      delay(15);
      calculationDist();
      Serial.print(pos);
      Serial.print(",");
      Serial.print(distance);
      Serial.print(".");

      if (distance < 40) {
        servoParado = true; 
        break;            
      }
    }
  }
}
