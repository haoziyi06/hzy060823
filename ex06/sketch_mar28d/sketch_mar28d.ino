// ex06: 警车双闪灯效（双通道反相 PWM）
#define LED_A 2    // 第一个 LED 接 GPIO2
#define LED_B 4    // 第二个 LED 接 GPIO4

void setup() {
  pinMode(LED_A, OUTPUT);  // 初始化 LED A 为输出
  pinMode(LED_B, OUTPUT);  // 初始化 LED B 为输出
}

void loop() {
  // 第一阶段：A 从暗到明，B 从明到暗
  for (int i = 0; i <= 255; i++) {
    analogWrite(LED_A, i);
    analogWrite(LED_B, 255 - i);
    delay(8);
  }

  // 第二阶段：A 从明到暗，B 从暗到明
  for (int i = 255; i >= 0; i--) {
    analogWrite(LED_A, i);
    analogWrite(LED_B, 255 - i);
    delay(8);
  }
}