// Step 3: PIR motion sensor -- digital again, but a different sensing
// principle and a quirk step 1 didn't have
// Breadboard wiring:
//   VCC -> 5V (most PIR modules, e.g. HC-SR501, want 5V)
//   GND -> GND
//   OUT -> GPIO27
//
// Where step 1's IR sensor reflects its own emitted IR off nearby objects,
// a PIR ("Passive" IR) module emits nothing -- it watches for *changes* in
// ambient infrared (body heat) crossing its lens. OUT goes HIGH while
// motion is seen. The quirk: most modules need ~30-60s after power-up to
// calibrate against the room's background IR before readings are reliable,
// and hold HIGH for a few seconds after motion stops (retrigger delay, set
// by an onboard pot) -- so don't expect instant on/off like step 1's OUT.

#define PIR_PIN 27
#define WARMUP_MS 30000

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  Serial.println("Warming up (~30s) -- ignore readings until this finishes...");
  delay(WARMUP_MS);
  Serial.println("Ready.");
}

void loop() {
  bool motion = digitalRead(PIR_PIN) == HIGH;
  Serial.println(motion ? "motion" : "still");
  delay(200);
}
