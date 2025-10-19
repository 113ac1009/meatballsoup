const int RledPin = 9;
const int GledPin = 10;
const int BledPin = 11;
const int buttonPin = 8;

int mood = 0;
const int neutralmood = 10;
unsigned long previousMillis = 0;
int fadingDirection = 1;
int buttonState = 0;
bool ButtonPressed = false;
unsigned long touchedTimer = 0;
unsigned long reducedTimer = 0;
const long unTouchInterval = 5000;
const long reducedInterval = 1000;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  mood = neutralmood;
  previousMillis = millis();
  reducedTimer = millis();
  touchedTimer = 0;
}

void loop() {

  float brightnessInterval = 255.0 / 10.0;
  if (mood >= neutralmood) {
    analogWrite(RledPin, 255);
    analogWrite(GledPin, brightnessInterval * (mood - neutralmood));
    analogWrite(BledPin, 255 - brightnessInterval * (mood - neutralmood));
  } else {
    analogWrite(RledPin, 255 - brightnessInterval * (neutralmood - mood));
    analogWrite(GledPin, brightnessInterval * (neutralmood - mood));
    analogWrite(BledPin, 255);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > 1000) {
    mood = mood + fadingDirection;
    if (mood >= 20 || mood <= 0) {
      fadingDirection = -fadingDirection;
    }
    previousMillis = currentMillis;
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && !ButtonPressed) {
    mood = mood + 1;
    if (mood > 20) mood = 20;
    touchedTimer = millis();
    ButtonPressed = true;
  }
  if (buttonState == LOW && ButtonPressed) {
    ButtonPressed = false;
  }

  unsigned long currentTimer = millis();
  if (currentTimer - touchedTimer > unTouchInterval) {
    if (currentTimer - reducedTimer > reducedInterval) {
      mood = mood - 1;
      if (mood < 0) mood = 0;
      reducedTimer = currentTimer;
    }
  }
}