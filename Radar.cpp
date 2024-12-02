#include<Servo.h>

// ----------------- Entradas -----------------
#define trigPin 9
#define echoPin 10

// ------------------ Entidades ------------------
long duration;
int distance ;
Servo myservo;
int pos = 0;

// ------------------ Distancia ------------------
int calculateDistance()
{
  digitalWrite(trigPin,LOW);
  //delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  //delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  return distance;
}

void setup()
{
  pinMode(trigPin , OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(11);
  Serial.begin(9600);
}

void loop()
{
 for (pos = 0; pos <= 180; pos += 1){
  myservo.write(pos);
  delay(15);
  calculateDistance();
  Serial.print(pos);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
 }
 for(pos = 180; pos >= 0; pos -= 1){
  myservo.write(pos);
  delay(15);
  calculateDistance();
  Serial.print(pos);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
 }
}
