// Step 5: DHT11 temperature + humidity -- a digital sensor, but a
// bit-banged protocol instead of a plain HIGH/LOW read or an analog level
// Breadboard wiring:
//   VCC -> 3.3V, GND -> GND, DATA -> GPIO26 (with a 10kohm pull-up to VCC --
//   many DHT11 breakout boards already include one)
//
// Written by hand instead of pulling in a library, so the protocol itself
// is visible: pull DATA low >=18ms to signal "start", release it, then the
// sensor answers with an 80us-low/80us-high handshake followed by 40 bits
// (5 bytes: humidity int, humidity frac, temp int, temp frac, checksum).
// Each bit is a ~50us low pulse followed by a high pulse whose *length*
// encodes the bit: ~26-28us = 0, ~70us = 1.

#define DHT_PIN 26

bool readDHT11(int &humidity, int &tempC) {
  uint8_t data[5] = {0, 0, 0, 0, 0};

  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(20);
  pinMode(DHT_PIN, INPUT_PULLUP);

  unsigned long t0 = micros();
  while (digitalRead(DHT_PIN) == HIGH) if (micros() - t0 > 100) return false;
  while (digitalRead(DHT_PIN) == LOW)  if (micros() - t0 > 300) return false;
  while (digitalRead(DHT_PIN) == HIGH) if (micros() - t0 > 400) return false;

  for (int i = 0; i < 40; i++) {
    unsigned long lowStart = micros();
    while (digitalRead(DHT_PIN) == LOW) if (micros() - lowStart > 200) return false;

    unsigned long highStart = micros();
    while (digitalRead(DHT_PIN) == HIGH) if (micros() - highStart > 200) return false;
    unsigned long width = micros() - highStart;

    data[i / 8] <<= 1;
    if (width > 40) data[i / 8] |= 1;  // longer high pulse -> bit 1
  }

  if (data[4] != (uint8_t)(data[0] + data[1] + data[2] + data[3])) return false;  // checksum
  humidity = data[0];
  tempC = data[2];
  return true;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int humidity, tempC;
  if (readDHT11(humidity, tempC)) {
    Serial.printf("%d%% RH, %dC\n", humidity, tempC);
  } else {
    Serial.println("read failed (DHT11 needs >=1s between reads)");
  }
  delay(1500);
}
