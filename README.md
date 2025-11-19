ESP32 DHT22 Temperature & Humidity Monitor with Flask Backend

This project allows an ESP32 device to read temperature and humidity data from a DHT22 sensor and send it to a Flask backend hosted on Vercel. Data is displayed on a 16x2 LCD and can be viewed and managed through your server.

Features

Real-time temperature and humidity monitoring via DHT22.

Display readings on a 16x2 I2C LCD.

Send JSON data to a Flask backend via HTTP POST.

Retry mechanism for failed readings or network issues.

WiFi reconnection handling.

Friendly LCD messages for setup, errors, and sending status.

Easy integration with web or mobile dashboards.

Hardware Requirements

ESP32 development board (any variant)

DHT22 temperature & humidity sensor

16x2 I2C LCD display

Breadboard and jumper wires

Power source (USB or battery)

Software Requirements

Arduino IDE or PlatformIO

ESP32 board libraries installed in Arduino IDE

DHTesp library for sensor readings

LiquidCrystal_I2C library for the LCD

WiFi.h and HTTPClient.h for networking

Wiring (https://wokwi.com/projects/448064114677287937)
ESP32 Pin	Component	Notes
14	DHT22 Data Pin	Connect data pin to GPIO14
3.3V	DHT22 VCC	Power pin
GND	DHT22 GND	Ground
SDA	LCD SDA	Usually GPIO21
SCL	LCD SCL	Usually GPIO22
3.3V	LCD VCC	Power pin
GND	LCD GND	Ground

Adjust SDA/SCL pins based on your board and LCD module.

Setup Instructions

Clone or download this repository.

Open esp32_dht_lcd.ino in Arduino IDE.

Update WiFi credentials:

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";


Update server URL to point to your Flask backend:

const char* serverUrl = "https://your-flask-backend.vercel.app/data";


Install necessary libraries via Arduino Library Manager:

DHTesp

LiquidCrystal_I2C

Select your ESP32 board and port in Arduino IDE.

Upload the code to your ESP32.

Open the Serial Monitor to see debug logs.

Flask Backend

A simple Flask backend receives JSON data:

{
  "temperature": 24.5,
  "humidity": 55.2,
  "device": "esp32-wokwi"
}


Example Flask route:

from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route("/data", methods=["POST"])
def receive_data():
    data = request.get_json()
    print(f"Received data: {data}")
    # You can store it in a database or file
    return jsonify({"success": True})

How It Works

ESP32 reads temperature and humidity from DHT22.

Displays the readings on the LCD.

Sends the readings as JSON to the Flask server.

Retries up to 3 times if sending fails.

Reconnects to WiFi automatically if disconnected.

Waits 60 seconds between readings.

Example LCD Output
Temp: 24.5C
Hum: 55.2%


or when sending:

Sending...

Troubleshooting

DHT read failed: Check wiring and sensor power.

WiFi disconnected: Verify SSID/password and signal strength.

Failed to connect to server: Check server URL and availability.

Future Improvements

Add motion sensor integration.

Store readings in a database.

Display history graphs in a web dashboard.

Add OTA updates for the ESP32 firmware.
