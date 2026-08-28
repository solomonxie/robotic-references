// Program 4: External LED through a current-limiting resistor
// Breadboard wiring:
//   GPIO4 -> 220ohm resistor -> LED anode (long leg)
//   LED cathode (short leg) -> GND
//
// Why 220ohm: resistor = (supply_voltage - LED_forward_voltage) / desired_current
//             (3.3V - ~2.0V) / 0.01A (10mA) = ~130ohm minimum; 220ohm is a safe
//             common value that keeps most LEDs comfortably lit without overdriving them.

#define LED_PIN 4
#define PAUSE 250  // delay in ms

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(PAUSE);
  digitalWrite(LED_PIN, LOW);
  delay(PAUSE);
}
