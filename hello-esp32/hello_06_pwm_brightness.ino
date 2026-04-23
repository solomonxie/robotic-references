// Program 6: Fade an LED's brightness with PWM
// Breadboard wiring: same as step 4 — GPIO4 -> 220ohm resistor -> LED anode,
// LED cathode -> GND.
//
// Uses the arduino-esp32 core v3 LEDC API: ledcAttach binds a pin directly
// to a PWM channel (no separate channel number needed like older cores).

#define LED_PIN 4
#define PWM_FREQ 5000     // Hz
#define PWM_RES  8        // bits -> duty range 0-255

void setup() {
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
}

void loop() {
  for (int duty = 0; duty <= 255; duty++) {   // fade in
    ledcWrite(LED_PIN, duty);
    delay(10);
  }
  for (int duty = 255; duty >= 0; duty--) {   // fade out
    ledcWrite(LED_PIN, duty);
    delay(10);
  }
}
