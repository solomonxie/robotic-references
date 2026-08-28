// Program 5: Read an analog value from a potentiometer
// Breadboard wiring:
//   Potentiometer outer pin 1 -> 3.3V
//   Potentiometer outer pin 2 -> GND
//   Potentiometer wiper (middle pin) -> GPIO34
//
// GPIO34 is an ADC1 input-only pin — a natural fit for a sensor input, and
// ADC1 (GPIO32-39) stays accurate even while WiFi is active, unlike ADC2.

#define POT_PIN 34

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(POT_PIN);  // 0-4095 (12-bit ADC)
  Serial.println(raw);
  delay(200);
}
