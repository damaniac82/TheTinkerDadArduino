#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "settings.h"

int incomingByte = 0;
WiFiClient espClient;
HTTPClient http;
 
void setup() {
 
  Serial.begin(9600);
  connectIfNeeded();
}

void connectIfNeeded() {

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
  }
}

void loop() {

  connectIfNeeded();

  // get data over rest - repeat this for whatever endpoints you want to fetch and display
  http.begin(NETDATA_URL);
  http.GET();
  String payload = http.getString();
  DynamicJsonBuffer  jsonBuffer(200);
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    Serial.println(">Service unavailable!<");
    return;
  }

  int uptime_secs_t = root["data"][0][1];
  int uptime_hours = uptime_secs_t / 60 / 60;
  int uptime_mins = (uptime_secs_t - uptime_hours * 60 * 60) / 60;
  int uptime_secs = (uptime_secs_t - uptime_hours * 60 * 60 - uptime_mins * 60);
  
  char buf[256];
  sprintf(buf, ">Uptime: %0d:%0d:%0d<", uptime_hours, uptime_mins, uptime_secs);
  Serial.println(buf);
  
  delay(1000);
}
