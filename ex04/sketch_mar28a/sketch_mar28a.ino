// ex04: 触摸自锁开关（摸一下亮，再摸灭）
#define TOUCH_PIN 4    // 触摸传感器接 GPIO4
#define LED_PIN   2    // LED 接 GPIO2（ESP32 板载 LED 通常是 2）

bool ledState = false;        // 记录 LED 当前状态
bool lastTouchState = false;  // 记录上一次触摸状态
unsigned long lastDebounceTime = 0;  // 上次触发时间
const unsigned long debounceDelay = 50;  // 防抖 50ms，防止手抖误触

void setup() {
  pinMode(LED_PIN, OUTPUT);   // 设置 LED 引脚为输出模式
  pinMode(TOUCH_PIN, INPUT);  // 设置触摸引脚为输入模式
  digitalWrite(LED_PIN, ledState);  // 初始化 LED 状态
}

void loop() {
  int currentTouch = digitalRead(TOUCH_PIN);  // 读取当前触摸值
  unsigned long now = millis();               // 获取当前时间

  // 防抖逻辑：只有状态变化且超过防抖时间，才认为是有效触发
  if (currentTouch != lastTouchState && (now - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = now;  // 更新最后触发时间

    // 检测「上升沿」：从松开（LOW）→ 触摸（HIGH）的瞬间
    if (currentTouch == HIGH) {
      ledState = !ledState;          // 翻转 LED 状态（亮 ↔ 灭）
      digitalWrite(LED_PIN, ledState);  // 把新状态写入 LED
    }
  }
  lastTouchState = currentTouch;  // 保存当前状态，供下一次循环对比
}