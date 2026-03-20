/// 定义三个LED的引脚
const int ledPin1 = 2;   // D2
const int ledPin2 = 4;   // D4
const int ledPin3 = 22;  // D22

// 记录每个LED上次状态变化的时间
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

// 闪烁间隔：500ms（1Hz = 1秒一个周期，半周期翻转一次）
const long interval = 500;

void setup() {
  // 初始化串口（可选，用于调试）
  Serial.begin(115200);
  
  // 将三个LED引脚设置为输出模式
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis(); // 获取当前系统运行毫秒数

  // 控制第一个LED（D2）
  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;
    int ledState = digitalRead(ledPin1);
    ledState = !ledState;
    digitalWrite(ledPin1, ledState);
    Serial.print("LED1 (D2): ");
    Serial.println(ledState ? "ON" : "OFF");
  }

  // 控制第二个LED（D4）
  if (currentMillis - previousMillis2 >= interval) {
    previousMillis2 = currentMillis;
    int ledState = digitalRead(ledPin2);
    ledState = !ledState;
    digitalWrite(ledPin2, ledState);
    Serial.print("LED2 (D4): ");
    Serial.println(ledState ? "ON" : "OFF");
  }

  // 控制第三个LED（D22）
  if (currentMillis - previousMillis3 >= interval) {
    previousMillis3 = currentMillis;
    int ledState = digitalRead(ledPin3);
    ledState = !ledState;
    digitalWrite(ledPin3, ledState);
    Serial.print("LED3 (D22): ");
    Serial.println(ledState ? "ON" : "OFF");
  }
}