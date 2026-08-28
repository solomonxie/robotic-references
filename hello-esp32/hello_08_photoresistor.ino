// Program 8: Light sensor (photoresistor/LDR) turns an LED on in the dark
// Breadboard wiring:
//   LDR leg 1 -> 3.3V
//   LDR leg 2 -> junction A
//   10kohm resistor: junction A -> GND        (LDR + resistor form a voltage divider)
//   junction A -> GPIO35 (ADC1, input-only)
//   GPIO4 -> 220ohm resistor -> LED anode -> ... -> LED cathode -> GND (from step 4)
//
// More light on the LDR -> lower its resistance -> higher voltage at junction A
// -> higher analogRead() value. Tune DARK_THRESHOLD to your room's lighting.

#define LDR_PIN 35
#define LED_PIN 4
#define DARK_THRESHOLD 1500  // below this reading, treat it as "dark"

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int light = analogRead(LDR_PIN);
  Serial.println(light);

  digitalWrite(LED_PIN, light < DARK_THRESHOLD ? HIGH : LOW);
  delay(200);
}
