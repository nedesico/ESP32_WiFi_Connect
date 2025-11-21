#include <ESP32_WiFi_Connect.h>

ESP32_WiFi_Connect espwifi; // Initialize the ESP32 WiFi Connect System

void setup() {
  Serial.begin(115200);

  espwifi.setCustomAP("MyCustomAPName", "MySup3r5ecurePass@99"); // Set your custom AP SSID and password
  // Password should be a minimum of 8 characters (secured), or set to "" for an open network (insecure)

  espwifi.begin();

  Serial.println(espwifi.getStatus()); // Get the device WiFi status.
  // wifi_not_configured, means the WiFi SSID hasn't been configured yet
  // wifi_connection_failed, means the WiFi SSID has been configured, but it can't connect.
  // - Check your settings, or whether the WiFi is out of range. Alternatively factory reset or format flash memory and reconfigure the device.
  // wifi_connected, means the WiFi SSID has been configured and is connected
}

void loop() {}
