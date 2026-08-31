// Step 1: IR obstacle avoidance sensor -- simplest possible sensor pattern
// Breadboard wiring:
//   VCC -> 3.3V, GND -> GND
//   OUT -> GPIO13
//
// This module (robo-car has 2, see ../robo-car/README.md) shines an IR LED
// and watches a phototransistor for the reflection. Its onboard comparator
// already turns that into a clean digital signal, output LOW when something
// is close enough to reflect IR back (threshold set by the onboard
// potentiometer), HIGH otherwise -- no analog math needed on our side, just
// digitalRead. This is the baseline every other sensor step builds past.

#define IR_PIN 13

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
}

void loop() {
  bool obstacle = digitalRead(IR_PIN) == LOW;  // active-low: LOW = something detected
  Serial.println(obstacle ? "obstacle" : "clear");
  delay(200);
}
