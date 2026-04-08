#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "张元英";
const char* password = "96518133";

WebServer server(80);

const int ledPin = 2;
const int touchPin = T0;

bool isArmed = false;
bool isAlarm = false;

const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

void IRAM_ATTR handleTouch() {
  if (isArmed && !isAlarm) {
    isAlarm = true;
  }
}

void setup() {
  Serial.begin(115200);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, 0);

  touchAttachInterrupt(touchPin, handleTouch, 40);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("ESP32 IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);

  server.begin();
}

void loop() {
  server.handleClient();

  if (isAlarm) {
    ledcWrite(ledChannel, 255);
    delay(100);
    ledcWrite(ledChannel, 0);
    delay(100);
  }
}

void handleRoot() {
  String status = isArmed ? "Armed" : "Disarmed";
  String alarm = isAlarm ? "ALARM!" : "Normal";

  String html = R"(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Security Alarm</title>
  <style>
    body { text-align: center; margin-top: 60px; font-family: Arial; }
    button { font-size: 26px; padding: 18px 35px; margin: 15px; }
    #arm { background: green; color: white; }
    #disarm { background: red; color: white; }
  </style>
</head>
<body>
  <h1>ESP32 Security System</h1>
  <p>System: <span id="status">)" + status + R"(</span></p >
  <p>Alarm: <span id="alarm">)" + alarm + R"(</span></p >
  <button id="arm" onclick="fetch('/arm');location.reload()">Arm</button>
  <button id="disarm" onclick="fetch('/disarm');location.reload()">Disarm</button>
</body>
</html>
  )";
  server.send(200, "text/html", html);
}

void handleArm() {
  isArmed = true;
  isAlarm = false;
  ledcWrite(ledChannel, 0);
  server.send(200, "text/plain", "OK");
}

void handleDisarm() {
  isArmed = false;
  isAlarm = false;
  ledcWrite(ledChannel, 0);
  server.send(200, "text/plain", "OK");
}