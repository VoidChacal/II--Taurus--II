#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= CONFIG =================
const char* ssid = "HUAWEI-2.4G-sDNj";
const char* password = "t3PeGAY4X@";

// HTTP Auth (apenas /cmd)
const char* HTTP_USER = "tau";
const char* HTTP_PASS = "senha_forte_123";

// Token global
const char* SECRET_TOKEN = "TAU-SECURE-TOKEN-001";

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// NRF24
RF24 radio(27, 14);
const byte address[6] = "00001";

// WebServer
WebServer server(80);

// ================= CHAT HANDLER =================
void handleChat() {

  // 🔐 Apenas TOKEN (sem Basic Auth)
  if (!server.hasHeader("X-TAU-TOKEN") ||
      server.header("X-TAU-TOKEN") != SECRET_TOKEN) {
    server.send(403, "text/plain", "Token inválido");
    return;
  }

  if (!server.hasArg("m")) {
    server.send(400, "text/plain", "Mensagem ausente");
    return;
  }

  String msg = server.arg("m");

  Serial.println("ECHO:" + msg);

  unsigned long start = millis();
  String response = "";

  while (millis() - start < 5000) {
  if (Serial.available()) {
    response = Serial.readStringUntil('\n');
    response.trim();

    if (response.startsWith("REPLY:")) {
      response.replace("REPLY:", "");
      break;
    }
  }
}

  if (response.length() == 0)
    server.send(504, "text/plain", "Timeout TAU");
  else
    server.send(200, "text/plain", response);
}


// ================= HTML =================
const char CHAT_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>TAU Proxy</title>
<style>
body { font-family: Arial; background:#111; color:#eee; }
#log { height:300px; overflow:auto; border:1px solid #444; padding:5px; }
input { width:80%; }
button { width:18%; }
</style>
</head>
<body>
<h2>TAU-VII-1 Proxy</h2>
<div id="log"></div><br>
<input id="msg" placeholder="Digite..." />
<button onclick="send()">Enviar</button>

<script>
function log(t){
  let d=document.getElementById("log");
  d.innerHTML+=t+"<br>";
  d.scrollTop=d.scrollHeight;
}

async function send(){
  let m=msg.value;
  msg.value="";
  log("Você: "+m);

  let r = await fetch("/chat?m="+encodeURIComponent(m), {
    headers: { "X-TAU-TOKEN": "TAU-SECURE-TOKEN-001" }
  });

  let t = await r.text();
  log("TAU: "+t);
}
</script>
</body>
</html>
)rawliteral";

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.print("ESP32 iniciando");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  lcd.clear();
  lcd.print("WiFi OK");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  // NRF
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.startListening();

  const char* headers[] = { "X-TAU-TOKEN" };
  server.collectHeaders(headers, 1);

  // ========== ROTAS ==========

 server.on("/", HTTP_GET, []() {
  server.send(200, "text/html", CHAT_HTML);
});

// Chat humano
server.on("/chat", HTTP_GET, handleChat);

// API segura (curl / Nomad)
server.on("/cmd", HTTP_GET, []() {

  if (!server.authenticate(HTTP_USER, HTTP_PASS)) {
    server.requestAuthentication();
    return;
  }

  if (!server.hasHeader("X-TAU-TOKEN") ||
      server.header("X-TAU-TOKEN") != SECRET_TOKEN) {
    server.send(403, "text/plain", "Token inválido");
    return;
  }

  if (!server.hasArg("c")) {
    server.send(400, "text/plain", "Parâmetro ausente");
    return;
  }

  String cmd = server.arg("c");
  Serial.println("ESP_CMD:" + cmd);

  unsigned long start = millis();
  String response = "";

  while (millis() - start < 4000) {
    if (Serial.available()) {
      response = Serial.readStringUntil('\n');
      response.trim();
      break;
    }
  }

  if (response.length() == 0)
    server.send(504, "text/plain", "Timeout Nomad");
  else
    server.send(200, "text/plain", response);
});

  server.begin();
}

// ================= LOOP =================
void loop() {
  server.handleClient();
}
