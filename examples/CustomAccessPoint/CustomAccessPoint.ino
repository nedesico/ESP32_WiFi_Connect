#include <ESP32_WiFi_Connect.h>
ESP32_WiFi_Connect espwifi;

void setup() {
  Serial.begin(115200);

  espwifi.debug(true); // Enable ESP32_WiFi_Connect debugging
  espwifi.setCustomAP("EasyTemps"); // Insecure Custom Access Point SSID, without password (insecure open network)
  //espwifi.setCustomAP("EasyTemps", "S3curePa55"); // Custom Access Point SSID, secured with password
  espwifi.begin();
}

void loop() {}
