// Step 7: sound detection module (e.g. KY-038) -- combines an analog
// envelope reading with a digital threshold trigger, unlike a USB
// microphone (which streams raw audio for software to process, not on the
// menu here -- that's an OS-level device, not a GPIO sensor)
// Breadboard wiring:
//   VCC -> 3.3V, GND -> GND
//   AOUT -> GPIO33 (raw sound envelope level)
//   DOUT -> GPIO25 (onboard comparator's threshold trigger)
//
// The onboard electret mic feeds an amplifier: AOUT tracks loudness
// continuously (like step 4's thermistor voltage), while an onboard
// comparator (threshold set by the module's own potentiometer) pulses DOUT
// LOW for a moment on a sudden loud sound -- a clap detector without any
// signal processing on our side.

#define SOUND_AOUT 33
#define SOUND_DOUT 25

void setup() {
  Serial.begin(115200);
  pinMode(SOUND_DOUT, INPUT);
}

void loop() {
  int level = analogRead(SOUND_AOUT);
  bool triggered = digitalRead(SOUND_DOUT) == LOW;  // active-low pulse on a loud sound

  Serial.print("level=");
  Serial.print(level);
  if (triggered) Serial.print("  <- LOUD SOUND");
  Serial.println();
  delay(100);
}
