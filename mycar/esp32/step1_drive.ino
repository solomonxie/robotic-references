// mycar Phase 1: Drive -- manual motor control, commanded over UART from the Pi
// Wiring:
//   L298N #1 (left side):  IN1->GPIO26, IN2->GPIO27, ENA->GPIO25 (PWM speed)
//   L298N #2 (right side): IN1->GPIO32, IN2->GPIO33, ENA->GPIO23 (PWM speed)
//   Pi link: ESP32 GPIO14 (RX2) <- Pi TXD, ESP32 GPIO15 (TX2) -> Pi RXD, shared GND
//            (both boards run 3.3V logic -- no level shifter needed)
//
// Protocol: single ASCII byte from the Pi over Serial2 @ 115200:
//   'F' forward, 'B' backward, 'L' turn left, 'R' turn right, 'S' stop
// Safety: no command for 500ms -> auto-stop, so a dropped WiFi/dashboard
// connection can't leave the car driving blind.

#define L_IN1 26
#define L_IN2 27
#define L_EN  25
#define R_IN1 32
#define R_IN2 33
#define R_EN  23

#define PI_RX 14  // ESP32 receives here (wire to Pi's TX)
#define PI_TX 15  // ESP32 transmits here (wire to Pi's RX)

#define WATCHDOG_MS 500
#define SPEED 200  // 0-255 PWM duty

unsigned long lastCommandAt = 0;

void setLeft(bool forward, int speed) {
  digitalWrite(L_IN1, forward ? HIGH : LOW);
  digitalWrite(L_IN2, forward ? LOW : HIGH);
  ledcWrite(L_EN, speed);
}

void setRight(bool forward, int speed) {
  digitalWrite(R_IN1, forward ? HIGH : LOW);
  digitalWrite(R_IN2, forward ? LOW : HIGH);
  ledcWrite(R_EN, speed);
}

void stopMotors() {
  digitalWrite(L_IN1, LOW);
  digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, LOW);
  digitalWrite(R_IN2, LOW);
  ledcWrite(L_EN, 0);
  ledcWrite(R_EN, 0);
}

void handleCommand(char cmd) {
  switch (cmd) {
    case 'F': setLeft(true, SPEED);  setRight(true, SPEED);  break;
    case 'B': setLeft(false, SPEED); setRight(false, SPEED); break;
    case 'L': setLeft(false, SPEED); setRight(true, SPEED);  break;  // skid turn left
    case 'R': setLeft(true, SPEED);  setRight(false, SPEED); break;  // skid turn right
    case 'S': stopMotors(); break;
    default: break;  // ignore unknown bytes
  }
}

void setup() {
  Serial2.begin(115200, SERIAL_8N1, PI_RX, PI_TX);

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);
  ledcAttach(L_EN, 5000, 8);
  ledcAttach(R_EN, 5000, 8);

  stopMotors();
}

void loop() {
  if (Serial2.available()) {
    handleCommand(Serial2.read());
    lastCommandAt = millis();
  }

  if (millis() - lastCommandAt > WATCHDOG_MS) {
    stopMotors();
  }
}
