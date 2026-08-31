// Step 8: WiFi -- sensing the RF environment with the ESP32's onboard radio
// No wiring: WiFi is built into the chip, no external module needed.
//
// Every step so far read a dedicated sensor part. This one reads the
// airwaves instead: WiFi.scanNetworks() has the radio listen for nearby
// access points' beacon frames and reports each one's name/signal strength
// -- no password/connection needed, just passive listening. This is the
// same radio robo-car's README flags as a possible alternative to the
// wired Pi<->ESP32 UART link (see its "Open assumptions" section).

#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // ensure a clean state before scanning
  delay(100);
}

void loop() {
  Serial.println("Scanning...");
  int found = WiFi.scanNetworks();

  if (found == 0) {
    Serial.println("No networks found.");
  } else {
    for (int i = 0; i < found; i++) {
      Serial.printf("%2d: %-32s RSSI=%d dBm  %s\n",
                     i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i),
                     WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "open" : "secured");
    }
  }
  WiFi.scanDelete();
  delay(5000);
}
