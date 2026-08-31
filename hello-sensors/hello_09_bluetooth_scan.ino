// Step 9: Bluetooth -- the ESP32's other onboard radio, same passive-scan
// idea as step 8 but for BLE (Bluetooth Low Energy) instead of WiFi
// No wiring: Bluetooth is built into the chip too. Uses the "ESP32 BLE
// Arduino" library bundled with the esp32 core -- no separate install.
//
// robo-car's Bluetooth speaker uses *classic* Bluetooth (A2DP audio, paired
// once via the Pi's Bluetooth chip -- see its README). BLE is a different,
// lower-power mode of the same radio standard, more common on sensor tags/
// beacons/wearables than on speakers. This scans for nearby BLE
// advertisements, same passive listening as step 8's WiFi scan.

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan *scanner;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  scanner = BLEDevice::getScan();
  scanner->setActiveScan(true);  // request scan response for more info per device
}

void loop() {
  Serial.println("Scanning...");
  BLEScanResults *results = scanner->start(5, false);  // 5-second scan window

  for (int i = 0; i < results->getCount(); i++) {
    BLEAdvertisedDevice device = results->getDevice(i);
    Serial.printf("%s  RSSI=%d dBm  %s\n",
                   device.getAddress().toString().c_str(), device.getRSSI(),
                   device.haveName() ? device.getName().c_str() : "(no name)");
  }
  scanner->clearResults();
  delay(2000);
}
