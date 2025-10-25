from flask import Flask, render_template_string, jsonify
import serial
import threading
import time
import requests

app = Flask(__name__)

# --- SERIAL CONFIG ---
SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE = 9600
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# --- TELEGRAM CONFIG ---
TELEGRAM_BOT_TOKEN = "83084xxxx:AAHDisbV1zI9ZNsKAkApRYBbwvPBhv8hYl"
TELEGRAM_CHAT_ID = "2410365265"
BILL_INTERVAL = 120 # 2 minutes = 1 month (for testing)

# --- DASHBOARD HTML ---
HTML = """
<!DOCTYPE html>
<html>
<head>
<title>Smart Energy Monitor</title>
<style>
body { font-family: Arial; background-color: #121212; color: white; text-align: center; padding: 20px; }
.card { background: #1e1e1e; border-radius: 12px; padding: 20px; width: 350px; margin: 15px auto; box-shadow: 0 0 10px #00ffcc; }
button { padding: 10px 20px; border: none; border-radius: 10px; font-size: 16px; margin: 5px; cursor: pointer; }
.on { background-color: #00ff99; }
.off { background-color: #ff4444; }
</style>
</head>
<body>
<h1>Smart Energy Monitor</h1>
<div class="card">
  <h3>Voltage: <span id="voltage">0</span> V</h3>
  <h3>Current: <span id="current">0</span> A</h3>
  <h3>Power: <span id="power">0</span> W</h3>
  <h3>Energy: <span id="energy">0</span> kWh</h3>
  <h3>Bill: <span id="bill">0</span> Rs</h3>
</div>
<div class="card">
  <h3>Relay Control</h3>
  <button class="on" onclick="toggleRelay('on')">Turn ON</button>
  <button class="off" onclick="toggleRelay('off')">Turn OFF</button>
</div>
<script>
function updateData() {
    fetch('/data').then(res => res.json()).then(data => {
        document.getElementById('voltage').innerText = data.voltage.toFixed(2);
        document.getElementById('current').innerText = data.current.toFixed(2);
        document.getElementById('power').innerText = data.power.toFixed(2);
        document.getElementById('energy').innerText = data.energy.toFixed(6);
        document.getElementById('bill').innerText = data.bill.toFixed(2);
    });
}
setInterval(updateData, 1000);

function toggleRelay(action) {
    fetch('/relay/' + action);
}
</script>
</body>
</html>
"""

# --- GLOBAL DATA ---
data_dict = {
    'voltage': 0,
    'current': 0,
    'power': 0,
    'energy': 0,
    'bill': 0,
}

# --- TELEGRAM BILL FUNCTION ---
def send_bill():
    while True:
        time.sleep(BILL_INTERVAL)
        message = (f"🧾 Smart Energy Bill Report:\n"
                   f"⚡Voltage: {data_dict['voltage']:.2f} V\n"
                   f"🔌Current: {data_dict['current']:.2f} A\n"
                   f"💡Power: {data_dict['power']:.2f} W\n"
                   f"🔋Energy: {data_dict['energy']:.6f} kWh\n"
                   f"💰Bill: {data_dict['bill']:.2f} Rs")
        url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/sendMessage"
        payload = {"chat_id": TELEGRAM_CHAT_ID, "text": message}
        try:
            requests.post(url, data=payload)
            print("Telegram bill sent!")
        except Exception as e:
            print("⚠️ Failed to send Telegram message:", e)

# --- FLASK ROUTES ---
@app.route('/')
def index():
    return render_template_string(HTML)

@app.route('/data')
def get_data():
    line = ser.readline().decode('utf-8').strip()
    if line:  # process only if not empty
        try:
            values = [float(x) for x in line.split(',')]
            if len(values) == 5:
                v, a, p, e, b = values
                # Update global data only when valid
                data_dict['voltage'] = v
                data_dict['current'] = a
                data_dict['power'] = p
                data_dict['energy'] = e
                data_dict['bill'] = b
        except:
            # ignore bad lines and keep last valid reading
            pass
    return jsonify(data_dict)

@app.route('/relay/<action>')
def relay(action):
    if action == 'on':
        ser.write(b'ON\n')
    elif action == 'off':
        ser.write(b'OFF\n')
    return '', 200

# --- MAIN ---
if __name__ == '__main__':
    telegram_thread = threading.Thread(target=send_bill)
    telegram_thread.daemon = True
    telegram_thread.start()
    app.run(host='0.0.0.0', port=5000)
