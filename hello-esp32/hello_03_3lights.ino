// Program 3: Cycle 3 LEDs (blue/red/green) one at a time
// Breadboard wiring:
//   Blue LED  anode -> GPIO18, cathode -> GND (through resistor)
//   Red LED   anode -> GPIO19, cathode -> GND (through resistor)
//   Green LED anode -> GPIO21, cathode -> GND (through resistor)

#define LED_BLUE  18
#define LED_RED   19
#define LED_GREEN 21
#define PAUSE 500  // delay in ms per color

void setup() {
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BLUE, HIGH);
  delay(PAUSE);
  digitalWrite(LED_BLUE, LOW);

  digitalWrite(LED_RED, HIGH);
  delay(PAUSE);
  digitalWrite(LED_RED, LOW);

  digitalWrite(LED_GREEN, HIGH);
  delay(PAUSE);
  digitalWrite(LED_GREEN, LOW);
}
