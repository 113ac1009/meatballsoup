const uint8_t LED_PIN = 3;   
const uint8_t BUTTON_PIN = 5; 

uint8_t mode = 0;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; 
int lastButtonReading = HIGH; 
int buttonState = HIGH;

unsigned long previousMillis = 0;
bool ledOn = true;

const unsigned long intervalForMode[4] = {
  0,    
  1000,
  500,  
  125  
};

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, HIGH);
  ledOn = true;
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        mode = (mode + 1) % 4;
        if (mode == 0) {
          digitalWrite(LED_PIN, HIGH);
          ledOn = true;
        } else {
          previousMillis = millis();
          ledOn = true;
          digitalWrite(LED_PIN, HIGH);
        }
      }
    }
  }

  lastButtonReading = reading;

  if (mode == 0) {
  } else {
    unsigned long interval = intervalForMode[mode];
    if (interval > 0) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      }
    }
  }
}
