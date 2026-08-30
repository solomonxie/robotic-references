"""robo-car Phase 1: Drive -- minimal web dashboard for manual control.

Run on the Pi (inside its venv, see README): python3 app.py
Then open http://<pi-hostname>.local:5000 from any device on the home WiFi.

Talks to the ESP32 over the Pi's GPIO UART (/dev/serial0, GPIO14/15).

NOTE: the Pi Zero W's Bluetooth (needed for the speaker, later phases) also
wants the good PL011 UART, which pushes GPIO14/15 onto the less stable
"mini UART". Add `core_freq=250` to /boot/config.txt so its baud rate stays
stable while Bluetooth is active too -- otherwise commands can get garbled.
"""
import serial
from flask import Flask

app = Flask(__name__)
ser = serial.Serial("/dev/serial0", 115200, timeout=1)

PAGE = """
<!doctype html>
<title>robo-car</title>
<style>
  body { font-family: sans-serif; text-align: center; margin-top: 2em; }
  button { font-size: 2em; width: 4em; height: 2em; margin: 0.2em; }
  .row { display: flex; justify-content: center; }
</style>
<div class="row"><button onclick="send('F')">&uarr;</button></div>
<div class="row">
  <button onclick="send('L')">&larr;</button>
  <button onclick="send('S')">&#9632;</button>
  <button onclick="send('R')">&rarr;</button>
</div>
<div class="row"><button onclick="send('B')">&darr;</button></div>
<script>
function send(cmd) { fetch('/cmd/' + cmd); }
</script>
"""


@app.route("/")
def index():
    return PAGE


@app.route("/cmd/<cmd>")
def cmd(cmd):
    if cmd in "FBLRS":
        ser.write(cmd.encode())
        return "ok"
    return "bad command", 400


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
