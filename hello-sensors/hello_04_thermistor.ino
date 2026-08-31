// Step 4: thermistor -- first analog sensor in this series, needs real math
// to turn a raw reading into a physical unit
// Breadboard wiring (voltage divider, same idea as hello-esp32's
// hello_08_photoresistor, an LDR):
//   3.3V -> thermistor -> junction A -> 10kohm resistor -> GND
//   junction A -> GPIO34 (ADC1, input-only)
//
// A thermistor's resistance falls as it warms up. Its exact curve varies by
// part, but most generic 10kohm NTC thermistors (like the one in this
// repo's Miuzei Fun Kit) are close enough to a "Beta" of ~3950 to estimate
// temperature from resistance -- treat this as a rough estimate, not a
// calibrated instrument.

#define THERM_PIN 34
#define R_FIXED 10000.0    // ohms, the fixed bottom resistor
#define R_NOMINAL 10000.0  // thermistor's resistance at 25C
#define T_NOMINAL 298.15   // 25C in Kelvin
#define BETA 3950.0        // typical for a generic 10k NTC

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(THERM_PIN);
  float voltage = raw / 4095.0 * 3.3;

  // Thermistor is on top (VCC side): Vout = Vcc * R_fixed / (R_therm + R_fixed)
  float r_therm = R_FIXED * (3.3 / voltage - 1);

  // Beta equation: 1/T = 1/T0 + (1/B) * ln(R/R0)
  float inv_t = 1.0 / T_NOMINAL + (1.0 / BETA) * log(r_therm / R_NOMINAL);
  float temp_c = (1.0 / inv_t) - 273.15;

  Serial.print(temp_c);
  Serial.println(" C");
  delay(500);
}
