// 定义三个LED的引脚
const int ledPin1 = 2;   // D2
const int ledPin2 = 4;   // D4
const int ledPin3 = 22;  // D22

// 状态机变量
unsigned long previousMillis = 0;  // 记录上次状态变化时间
int currentStep = 0;               // 当前执行到SOS序列的第几步
bool ledState = HIGH;              // 当前LED状态（初始为亮）
unsigned long currentDuration;      // 当前状态需要持续的时长

// SOS 时间序列：(亮时长ms, 灭时长ms)
// 顺序：3次短闪 → 3次长闪 → 3次短闪 → 结束停顿
const int sosSequence[][2] = {
  {200, 200}, {200, 200}, {200, 200},  // 3次短闪 (S)
  {600, 200}, {600, 200}, {600, 200},  // 3次长闪 (O)
  {200, 200}, {200, 200}, {200, 2000}  // 3次短闪 (S) + 结尾长停顿(2000ms灭)
};
const int totalSteps = sizeof(sosSequence) / sizeof(sosSequence[0]);

void setup() {
  Serial.begin(115200);  // 串口调试（可选）
  
  // 将三个LED引脚设置为输出模式
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  
  // 初始化：点亮LED，开始第一个亮阶段
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  ledState = HIGH;
  currentStep = 0;
  currentDuration = sosSequence[currentStep][1]; // 第一个亮之后应等待灭时间
  previousMillis = millis();
  
  Serial.println("SOS Started");
}

void loop() {
  unsigned long currentMillis = millis();

  // 检查是否达到当前状态的持续时间
  if (currentMillis - previousMillis >= currentDuration) {
    previousMillis = currentMillis;  // 更新时间戳

    if (ledState == HIGH) {
      // 亮阶段结束，熄灭LED，进入灭阶段（当前步骤的灭）
      ledState = LOW;
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      Serial.println("ALL LED OFF");
      // 下一个持续时间：当前步骤的灭时间
      currentDuration = sosSequence[currentStep][1];
    } else {
      // 灭阶段结束，点亮LED，进入下一个步骤的亮阶段
      currentStep = (currentStep + 1) % totalSteps;  // 前进到下一步
      ledState = HIGH;
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      Serial.println("ALL LED ON");
      // 下一个持续时间：新步骤的亮时间
      currentDuration = sosSequence[currentStep][0];
    }
  }
}