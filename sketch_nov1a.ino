// JoyStick Shield pins
const int JoyStick_pin = 8;
const int X_pin = A0;
const int Y_pin = A1;
int xc;
int yc;
int JSButton;

void setup() {
  pinMode(JoyStick_pin, INPUT);
  Serial.begin(115200);
}

void loop() {
  int x = analogRead(X_pin) - 512;
  int y = analogRead(Y_pin) - 512;

  if (x < -50) {      // Ajuste para uma sensibilidade mais alta
    xc = 0;           // Movimento para a esquerda
  } else if (x > 50) {
    xc = 2;           // Movimento para a direita
  } else {
    xc = 1;           // Neutro
  }

  if (y < -50) {
    yc = 0;           // Movimento para cima
  } else if (y > 50) {
    yc = 2;           // Movimento para baixo
  } else {
    yc = 1;           // Neutro
  }

  int buttonStates = ((digitalRead(JoyStick_pin) == LOW) ? 1 : 0);

  Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.print(y);
  Serial.print(" XC: "); Serial.print(xc);
  Serial.print(" YC: "); Serial.print(yc);
  Serial.print(" Button: "); Serial.println(buttonStates);

  delay(40);
}
