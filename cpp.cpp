#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h> 

bool motionSent = false;
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverUrl = "https://esp-32-motion-detector.vercel.app/data";

#define pir_sensor 26
#define led_pin 22

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(pir_sensor, INPUT);
  pinMode(led_pin, OUTPUT);

 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }

  
  configTime(0, 0, "pool.ntp.org");
}

String getTimestamp() {
  time_t now;
  time(&now);
  struct tm* timeinfo = localtime(&now);
  char buf[25];
  strftime(buf, 25, "%Y-%m-%dT%H:%M:%SZ", timeinfo);
  return String(buf);
}

void send_pir_data(bool motionDetected) {
  digitalWrite(led_pin, HIGH);

  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("User-Agent", "ESP32-Wokwi");
  http.setTimeout(10000);

  String timestamp = getTimestamp();
  String payload = String("{\"motion_detected\":") + (motionDetected ? "true" : "false") +
                   String(",\"timestamp\":\"") + timestamp + String("\"}");

  Serial.println("Sending data: " + payload);

  int httpCode = -1;
  for (int i = 0; i < 3; i++) { 
    Serial.printf("POST attempt %d...\n", i + 1);
    httpCode = http.POST(payload);
    if (httpCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpCode);
      break;
    } else {
      Serial.printf("POST failed, error: %s\n", http.errorToString(httpCode).c_str());
      if (i < 2) {
        Serial.println("Retrying in 2s...");
        delay(2000);
      }
    }
  }

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.println("Failed to connect to server after 3 attempts");
  }

  http.end();
  digitalWrite(led_pin, LOW);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.reconnect();
    delay(5000);
    return;
  }

  int pir_state = digitalRead(pir_sensor);

  if (pir_state == HIGH && !motionSent) {
    motionSent = true;
    send_pir_data(true);
  }

  if (pir_state == LOW) {
    motionSent = false; 
  }

  delay(1000); 
}
