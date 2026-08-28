// Program 2: Read Pushbutton State

#define PB_PIN 18       // Pushbutton connected to GPIO18

void setup() {
  Serial.begin(115200);              // Start Serial Monitor at 115200 baud
  pinMode(PB_PIN, INPUT_PULLUP);     // GPIO18 as input with internal pull-up
}

void loop() {
  int state = digitalRead(PB_PIN);   // Read button: HIGH = not pressed, LOW = pressed

  if (state == LOW) {
    Serial.println("Button PRESSED");
  } else {
    Serial.println("Button NOT pressed");
  }

  delay(200);  // Small delay to avoid flooding Serial Monitor
}
