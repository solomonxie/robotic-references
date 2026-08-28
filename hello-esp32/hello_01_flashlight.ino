// Program 1: Flashing an LED light
// No breadboard wiring needed: GPIO2 drives the onboard LED on most ESP32 dev boards.
#define LED_PIN 2  // GPIO2
#define PAUSE 250  // delay in ms

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn on LED
  delay(PAUSE);
  digitalWrite(LED_PIN, LOW);   // Turn LED off
  delay(PAUSE);
}
