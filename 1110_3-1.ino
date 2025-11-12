const int buttonPin = 4;
const int pinR = 5; // R
const int pinG = 7; // G
const int pinB = 6; // B

// 按鍵與長短按參數
const unsigned long debounceDelay = 50;
const unsigned long shortPressMax = 600;
const unsigned long longPressMin = 1200;

// 顏色表 (R,G,B) 0-255
const int colors[][3] = {
  {255, 100, 24},   // 橘
  {0, 255, 0},      // 綠
  {50, 150, 248},   // 藍
  {255, 0, 0},      // 紅
  {255, 255, 255}   // 白
};
const int colorCount = sizeof(colors) / sizeof(colors[0]);

// 狀態
int colorIndex = 0;
bool specialMode = false; // 長按啟動的模式（閃爍 / 呼吸）
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long buttonDownTime = 0;
bool buttonPressed = false;

// 用於閃爍/呼吸效果
unsigned long lastEffectMillis = 0;
int breathStep = 0;
int breathDir = 1;

void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP); // 假設按鍵對 GND，使用內部上拉
  // 若按鍵接到 VCC，改用 INPUT 並處理外部下拉
  setColor(colors[colorIndex]);
}

void loop() {
  int rawState = digitalRead(buttonPin); // HIGH when not pressed if INPUT_PULLUP
  int reading = rawState;
  unsigned long now = millis();

  // 反向按鍵邏輯（內部上拉時按下為 LOW）
  int logicalState = (reading == LOW) ? HIGH : LOW;

  // 去抖動
  if (logicalState != lastButtonState) {
    lastDebounceTime = now;
  }

  if ((now - lastDebounceTime) > debounceDelay) {
    // 穩定狀態
    if (logicalState == HIGH && !buttonPressed) {
      // 按下事件開始
      buttonPressed = true;
      buttonDownTime = now;
    } else if (logicalState == LOW && buttonPressed) {
      // 放開事件 -> 決定短按或長按
      unsigned long pressDuration = now - buttonDownTime;
      buttonPressed = false;

      if (pressDuration >= longPressMin) {
        // 長按動作：切換 special 模式 on/off
        specialMode = !specialMode;
      } else if (pressDuration <= shortPressMax) {
        // 短按動作：切換顏色
        colorIndex = (colorIndex + 1) % colorCount;
        setColor(colors[colorIndex]);
      }
    }
  }

  lastButtonState = logicalState;

  // 如果 specialMode 開啟，執行效果（這裡為呼吸效果）
  if (specialMode) {
    runBreathEffect();
  } else {
    // 若不在 special 模式，保持目前選定顏色（若正在按下可可視需求立即變色）
    // setColor 已在短按處理時設定；但若外部因素改變，可在此確保
    setColor(colors[colorIndex]);
  }
}

// 設定固定顏色 (0-255)
void setColor(const int rgb[3]) {
  analogWrite(pinR, rgb[0]);
  analogWrite(pinG, rgb[1]);
  analogWrite(pinB, rgb[2]);
}

// 呼吸效果：以目前 colorIndex 的色彩做強度變化
void runBreathEffect() {
  unsigned long now = millis();
  const unsigned long stepDelay = 12; // 控制平滑度
  if (now - lastEffectMillis < stepDelay) return;
  lastEffectMillis = now;

  // breathStep 在 0..255 間往返
  breathStep += breathDir;
  if (breathStep <= 0) {
    breathStep = 0;
    breathDir = 1;
  } else if (breathStep >= 255) {
    breathStep = 255;
    breathDir = -1;
  }

  // 根據基色計算當前亮度
  int r = (colors[colorIndex][0] * breathStep) / 255;
  int g = (colors[colorIndex][1] * breathStep) / 255;
  int b = (colors[colorIndex][2] * breathStep) / 255;
  analogWrite(pinR, r);
  analogWrite(pinG, g);
  analogWrite(pinB, b);
}