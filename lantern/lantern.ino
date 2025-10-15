const int buttonPin = 4;
const int pinR = 5; 
const int pinG = 7; 
const int pinB = 6; 

int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

int colorIndex = 0; 

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  setColor(colorIndex); 
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    static int stableState = HIGH;
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) {
        colorIndex++;
        if (colorIndex > 3) colorIndex = 0; 
        setColor(colorIndex);
      }
    }
  }

  lastButtonState = reading;
}

void setColor(int idx) {
  switch (idx) {
    case 0: // 白
      analogWrite(pinR, 255);
      analogWrite(pinG, 255);
      analogWrite(pinB, 255);
      break;
    case 1: // 藍色
      analogWrite(pinR, 255);
      analogWrite(pinG, 100);
      analogWrite(pinB, 24);
      break;
    case 2: // 綠色
      analogWrite(pinR, 0);
      analogWrite(pinG, 255);
      analogWrite(pinB, 0);
      break;
    case 3: // 粉紅色
      analogWrite(pinR, 50);
      analogWrite(pinG, 150);
      analogWrite(pinB, 248);
      break;
    default:
      analogWrite(pinR, 255);
      analogWrite(pinG, 255);
      analogWrite(pinB, 255);
      break;
  }
}