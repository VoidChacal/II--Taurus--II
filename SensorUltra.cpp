const int trigPin = 9; 
const int echoPin = 10; 

long duration; // Duração do pulso ECHO em microsegundos
float distance; // Distância calculada em centímetros

void setup() {
  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  
  Serial.begin(9600); 
}

void loop() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  distance = (duration * 0.034) / 2; 

  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
