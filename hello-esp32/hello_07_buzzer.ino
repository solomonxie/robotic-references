// Program 7: Beep an active buzzer module
// Breadboard wiring (3-pin active buzzer module):
//   VCC -> 3.3V
//   GND -> GND
//   I/O (signal) -> GPIO5
//
// An *active* buzzer has its own driver circuit and just needs HIGH/LOW —
// it beeps at its own fixed tone. A *passive* buzzer instead needs a PWM
// tone (see step 6's ledcWrite) to produce sound.

#define BUZZER_PIN 5
#define PAUSE 500  // delay in ms

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);  // beep on
  delay(PAUSE);
  digitalWrite(BUZZER_PIN, LOW);   // beep off
  delay(PAUSE);
}
