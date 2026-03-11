#include <SoftwareSerial.h>

#define MQ2_PIN      A0
#define BUZZER_PIN   8
#define ESP_RX_PIN   2
#define ESP_TX_PIN   3

SoftwareSerial espSerial(ESP_RX_PIN, ESP_TX_PIN);

// WiFi + Blynk
#define WIFI_SSID     "TestWifi"
#define WIFI_PASSWORD "12345678"
#define AUTH_TOKEN    "Oja_QL5vG_F9f1h1pW8hq5qzaREQcCXW"
#define BLYNK_HOST    "blr1.blynk.cloud"

const int GAS_THRESHOLD = 400;
const int SEND_INTERVAL = 500;   // faster detection (0.5 sec)

bool wifiConnected = false;
bool alertSent = false;

unsigned long lastSendTime = 0;

void setup() {

  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println(F("================================"));
  Serial.println(F(" Gas Leak Detection System "));
  Serial.println(F("================================"));

  Serial.println(F("Warming MQ-2 sensor..."));
  warmupSensor(20);

  Serial.println(F("Connecting WiFi..."));
  wifiConnected = setupWiFi();

  if (wifiConnected) {
    Serial.println(F("System READY"));
    beep(2);
  } 
  else {
    Serial.println(F("WiFi FAILED - Local alarm mode"));
    beep(3);
  }
}

void loop() {

  unsigned long now = millis();

  if (now - lastSendTime >= SEND_INTERVAL) {

    lastSendTime = now;

    int gasValue = analogRead(MQ2_PIN);
    int isLeak = (gasValue >= GAS_THRESHOLD);

    Serial.println("------------------------------");
    Serial.print("Gas Value: ");
    Serial.println(gasValue);

    if (isLeak)
      Serial.println("⚠ GAS LEAK DETECTED!");
    else
      Serial.println("Safe");

    Serial.println("------------------------------");


    // 🚨 Instant Alarm
    if (isLeak) {

      digitalWrite(BUZZER_PIN, HIGH);

      if (wifiConnected && !alertSent) {

        sendNotification();
        alertSent = true;
      }
    }

    else {

      digitalWrite(BUZZER_PIN, LOW);
      alertSent = false;
    }


    // Send data to Blynk dashboard
    if (wifiConnected) {

      sendPin("V0", gasValue);
      sendPin("V1", isLeak);
    }
  }
}

// =========================================
// SEND DATA TO BLYNK DASHBOARD
// =========================================
bool sendPin(const char* pin, int value) {

  espSerial.println(F("AT+CIPCLOSE"));
  delay(200);

  espSerial.print(F("AT+CIPSTART=\"TCP\",\""));
  espSerial.print(F(BLYNK_HOST));
  espSerial.println(F("\",80"));

  if (!waitForResponse("CONNECT", 5000))
    return false;

  char path[80];

  snprintf(path, sizeof(path),
           "/external/api/update?token=%s&%s=%d",
           AUTH_TOKEN, pin, value);

  char request[140];

  snprintf(request, sizeof(request),
           "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
           path, BLYNK_HOST);

  espSerial.print(F("AT+CIPSEND="));
  espSerial.println(strlen(request));

  if (!waitForResponse(">", 3000))
    return false;

  espSerial.print(request);

  waitForResponse("SEND OK", 5000);

  espSerial.println(F("AT+CIPCLOSE"));

  return true;
}

// =========================================
// SEND PHONE NOTIFICATION
// =========================================
void sendNotification() {

  espSerial.println("AT+CIPCLOSE");
  delay(200);

  espSerial.println("AT+CIPSTART=\"TCP\",\"blr1.blynk.cloud\",80");

  if (!waitForResponse("CONNECT", 5000))
    return;

  String request =
  "GET /external/api/logEvent?token=Oja_QL5vG_F9f1h1pW8hq5qzaREQcCXW&code=gas_leak HTTP/1.1\r\n"
  "Host: blr1.blynk.cloud\r\n"
  "Connection: close\r\n\r\n";

  espSerial.print("AT+CIPSEND=");
  espSerial.println(request.length());

  if (!waitForResponse(">", 3000))
    return;

  espSerial.print(request);

  waitForResponse("SEND OK", 5000);

  espSerial.println("AT+CIPCLOSE");

  Serial.println("📱 Notification Sent");
}

// =========================================
// WIFI SETUP
// =========================================
bool setupWiFi() {

  delay(2000);

  if (!sendATCommand("AT", "OK", 3000)) {
    Serial.println("ESP not responding");
    return false;
  }

  espSerial.println("AT+RST");
  delay(5000);

  sendATCommand("AT+CWMODE=1", "OK", 3000);

  espSerial.print("AT+CWJAP=\"");
  espSerial.print(WIFI_SSID);
  espSerial.print("\",\"");
  espSerial.print(WIFI_PASSWORD);
  espSerial.println("\"");

  if (!waitForResponse("WIFI GOT IP", 20000))
    return false;

  Serial.println("WiFi Connected!");

  return true;
}

// =========================================
// SENSOR WARMUP
// =========================================
void warmupSensor(int seconds) {

  for (int i = seconds; i > 0; i--) {

    Serial.print("Warmup ");
    Serial.print(i);
    Serial.println(" sec");

    delay(1000);
  }

  Serial.println("Sensor Ready!");
}

// =========================================
// BUZZER BEEP
// =========================================
void beep(int times) {

  for (int i = 0; i < times; i++) {

    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);

    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
}

// =========================================
// AT COMMAND HELPERS
// =========================================
bool sendATCommand(String command, String expected, int timeoutMs) {

  espSerial.println(command);

  return waitForResponse(expected, timeoutMs);
}

bool waitForResponse(String expected, int timeoutMs) {

  String response = "";

  unsigned long start = millis();

  while (millis() - start < timeoutMs) {

    while (espSerial.available()) {

      char c = espSerial.read();
      response += c;

      Serial.print(c);
    }

    if (response.indexOf(expected) != -1)
      return true;

    if (response.indexOf("ERROR") != -1)
      return false;
  }

  return false;
}