// ex05: 三档触摸调速呼吸灯
#define TOUCH_PIN 4    // 触摸传感器接 GPIO4
#define LED_PIN   2    // LED 接 GPIO2

int brightness = 0;       // LED 亮度值（0~255）
int fadeStep = 5;         // 亮度每次变化的步长
int speedLevel = 1;       // 速度档位：1=慢，2=中，3=快
bool lastTouch = false;   // 上一次触摸状态
unsigned long lastDebounce = 0;  // 防抖时间记录
const unsigned long debounceDelay = 50;  // 防抖 50ms

void setup() {
  pinMode(LED_PIN, OUTPUT);   // 设置 LED 引脚为输出模式
  pinMode(TOUCH_PIN, INPUT);  // 设置触摸引脚为输入模式
}

void loop() {
  // 1. 触摸防抖 + 切换档位逻辑
  int currentTouch = digitalRead(TOUCH_PIN);
  unsigned long now = millis();

  if (currentTouch != lastTouch && (now - lastDebounce) > debounceDelay) {
    lastDebounce = now;
    if (currentTouch == HIGH) {
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1;  // 循环 1→2→3→1
    }
  }
  lastTouch = currentTouch;

  // 2. 呼吸灯核心逻辑
  analogWrite(LED_PIN, brightness);
  brightness += fadeStep;

  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
  }

  // 3. 根据档位控制呼吸速度
  int delayTime;
  if (speedLevel == 1) delayTime = 20;   // 慢速
  else if (speedLevel == 2) delayTime = 10;  // 中速
  else delayTime = 3;    // 快速
  delay(delayTime);
}