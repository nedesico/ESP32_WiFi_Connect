#include <ESP32_WiFi_Connect.h>

ESP32_WiFi_Connect espwifi;
void setup() {
  Serial.begin(115200);
  espwifi.begin();
  Serial.println(espwifi.getStatus());
}

void loop() {
  espwifi.keepAlive();
}
