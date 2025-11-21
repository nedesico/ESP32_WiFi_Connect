#include <ESP32_WiFi_Connect.h>
ESP32_WiFi_Connect espwifi;

void setup() {
  Serial.begin(115200);

  espwifi.debug(true); // Enable ESP32_WiFi_Connect debugging
  // espwifi.setCustomAP("EasyTemps"); // Insecure Custom Access Point SSID, without password (insecure open network)
  espwifi.setCustomAP("EasyTemps", "S3curePa55"); // Custom Access Point SSID, secured with password
  espwifi.begin();

  // Generate a custom "Status" page on url /status
  espwifi.onGet("/status", [](AsyncWebServerRequest *request) {
    espwifi.pageTitle("Status");
    String content = "<ul>";
    content += "<li style=\"clear:both;\">Chip model: <span style=\"display:inline-block;float:right;white-space:nowrap;\">" + String(ESP.getChipModel()) + "</span></li>";
    content += "<li style=\"clear:both;\">WiFi SSID: <span style=\"display:inline-block;float:right;white-space:nowrap;\">" + String(WiFi.SSID()) + "</span></li>";
    String rssiStrength = (WiFi.RSSI() < -60) ? "Weak" : "Strong"; 
    content += "<li style=\"clear:both;\">Strength: <span style=\"display:inline-block;float:right;white-space:nowrap;\">" + rssiStrength + "</span></li>";
    content += "<li style=\"clear:both;\">IP address: <span style=\"display:inline-block;float:right;white-space:nowrap;\">" + WiFi.localIP().toString() + "</span></li>";
    content += "<li style=\"clear:both;\">Status: <span style=\"display:inline-block;float:right;white-space:nowrap;\">" + String(espwifi.getStatus()) + "</span></li>";
    content += "</ul>";
    espwifi.sendHtmlPage(request, content);
  });
  espwifi.setDashboard("/status"); // Make the Status page your dashboard page (Optional. Your device will default to /wifi if setDashboard("/someurl") isn't set anywhere)
}

void loop() {}
