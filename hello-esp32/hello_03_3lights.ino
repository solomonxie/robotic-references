// Program 3: Cycle 3 LEDs (blue/red/green) one at a time
// Wiring: GPIO18 = Blue, GPIO19 = Red, GPIO21 = Green, all sharing one GND rail.

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
