#include <WiFi.h>
#include <WebServer.h>

// ===================== 【必须修改】WiFi配置区 =====================
const char* ssid = "张元英";
const char* password = "96518133";
// ======================================================================

WebServer server(80);

const int ledPin = 2;
const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;
int currentBrightness = 0;

void setup() {
  Serial.begin(115200);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("ESP32 IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Dimmer</title>
  <style>
    body { text-align: center; margin-top: 60px; font-family: Arial; }
    input[type=range] { width: 85%; height: 35px; }
  </style>
</head>
<body>
  <h1>ESP32 Web Dimmer</h1>
  <input type="range" min="0" max="255" value=")" + String(currentBrightness) + R"(" oninput="updateBrightness(this.value)">
  <h2>Brightness: <span id="val">)" + String(currentBrightness) + R"(</span></h2>
  <script>
    function updateBrightness(val) {
      document.getElementById("val").innerText = val;
      fetch("/set?value=" + val);
    }
  </script>
</body>
</html>
  )";
  server.send(200, "text/html", html);
}

void handleSetBrightness() {
  if (server.hasArg("value")) {
    int val = server.arg("value").toInt();
    if (val >= 0 && val <= 255) {
      currentBrightness = val;
      ledcWrite(ledChannel, currentBrightness);
    }
  }
  server.send(200, "text/plain", "OK");
}