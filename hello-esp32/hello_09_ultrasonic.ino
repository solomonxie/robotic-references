// Program 9: Measure distance with an HC-SR04 ultrasonic sensor
// Breadboard wiring:
//   VCC  -> 5V (HC-SR04 needs 5V to work reliably)
//   GND  -> GND
//   TRIG -> GPIO12
//   ECHO -> voltage divider -> GPIO14
//
// IMPORTANT: ECHO outputs 5V, but ESP32 GPIOs are only 3.3V-tolerant.
// Feeding ECHO straight into a GPIO risks damaging the pin. Step it down
// with a divider: ECHO -> 1kohm resistor -> GPIO14 -> 2kohm resistor -> GND
// (this yields ECHO * 2/3 ≈ 3.3V when ECHO is 5V).

#define TRIG_PIN 12
#define ECHO_PIN 14

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);   // 10us pulse tells the sensor to ping
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);  // us until the echo returns
  float distance_cm = duration / 58.0;      // standard HC-SR04 conversion

  Serial.print(distance_cm);
  Serial.println(" cm");
  delay(200);
}
