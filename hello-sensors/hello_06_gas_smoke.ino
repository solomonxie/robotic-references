// Step 6: MQ-2 gas/smoke sensor -- analog again, but needs calibration
// against a baseline instead of a fixed threshold
// Breadboard wiring:
//   VCC -> 5V, GND -> GND
//   AOUT -> GPIO32
//
// MQ-series sensors work by heating a metal-oxide element whose resistance
// shifts in the presence of smoke/combustible gas -- that heater needs a
// warm-up period (~20s here for a quick demo; real deployments burn it in
// for 24-48h once, then ~20s per power-cycle) before readings settle. There
// is no fixed "safe" ADC value across sensors/environments, so this
// captures a clean-air baseline first and flags a rise above it, rather
// than assuming a hardcoded threshold like step 1's obstacle sensor could.

#define GAS_PIN 32
#define WARMUP_MS 20000
#define ALERT_MARGIN 300  // ADC counts above baseline before flagging

void setup() {
  Serial.begin(115200);
  Serial.println("Warming up (~20s)...");
  delay(WARMUP_MS);
}

void loop() {
  static int baseline = -1;
  int raw = analogRead(GAS_PIN);

  if (baseline < 0) {
    baseline = raw;  // first reading after warm-up = "clean air" reference
    Serial.print("Baseline set: ");
    Serial.println(baseline);
    return;
  }

  Serial.print(raw);
  if (raw > baseline + ALERT_MARGIN) {
    Serial.println("  <- SMOKE/GAS above baseline");
  } else {
    Serial.println();
  }
  delay(500);
}
