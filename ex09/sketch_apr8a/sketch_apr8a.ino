#include <WiFi.h>
#include <WebServer.h>

// ===================== 【必须修改】WiFi配置区 =====================
const char* ssid = "张元英";
const char* password = "96518133";
// ======================================================================

WebServer server(80);

const int touchPin = T0;
int touchValue = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("ESP32 IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/getData", handleGetData);

  server.begin();
}

void loop() {
  touchValue = touchRead(touchPin);
  server.handleClient();
  delay(50);
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 实时传感器仪表盘</title>
  <style>
    body { 
      display: flex; 
      flex-direction: column; 
      align-items: center; 
      justify-content: center; 
      height: 90vh; 
      font-family: Arial; 
      background: #f0f0f0; 
    }
    .gauge { 
      font-size: 80px; 
      font-weight: bold; 
      color: #2c3e50; 
      margin: 30px; 
    }
    .title { 
      font-size: 36px; 
      color: #34495e; 
    }
    .desc { 
      font-size: 18px; 
      color: #7f8c8d; 
      margin-top: 20px; 
    }
  </style>
</head>
<body>
  <h1 class="title">ESP32 触摸传感器实时仪表盘</h1>
  <div class="gauge" id="valueDisplay">0</div>
  <p class="desc">手靠近数值变小，手离开数值恢复</p >

  <script>
    setInterval(() => {
      fetch("/getData")
        .then(res => res.text())
        .then(data => {
          document.getElementById("valueDisplay").innerText = data;
        });
    }, 100);
  </script>
</body>
</html>
  )";

  server.send(200, "text/html", html);
}

void handleGetData() {
  server.send(200, "text/plain", String(touchValue));
}