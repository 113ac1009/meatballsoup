void setup() {
  pinMode(5, OUTPUT);  // R
  pinMode(7, OUTPUT); // G
  pinMode(6, OUTPUT); // B
}

void loop() {
  // 藍色
  analogWrite(5, 255);
  analogWrite(7, 100);
  analogWrite(6, 24);
  delay(500);

  // 紫色
  analogWrite(5, 0);
  analogWrite(7,255);
  analogWrite(6, 0);
  delay(500);

  // 粉紅色
  analogWrite(5, 50);
  analogWrite(7, 150);
  analogWrite(6, 248);
  delay(500);
}