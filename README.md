# ESP32_WiFi_Connect

ESP32 WiFi Connect UI. Your ESP32 will automatically launch a dedicated WiFi Access Point DNS Server, with User Interface, when WiFi isn't configured yet or unavailable. This will allow you to configure and save the WiFi credentials to the onboard flash and connect to the WiFi when in range.

# Very basic usage:

```
#include <ESP32_WiFi_Connect.h>

ESP32_WiFi_Connect espwifi;

void setup() {
  Serial.begin(115200);

  espwifi.begin();

  Serial.println(espwifi.getStatus()); // optional debugging info
}

void loop() {}
```


Automatically creates a WiFi Access Point (AP) to allow you to connect to the device.

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/wifiAP.png">


Automatically scans for local WiFi networks and allows you to easily configure your local WiFi network credentials via a web app interface

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/wifiConfig1.png">


You can access the web app configuration using your AP SSID. Example: http://easyesp-3e76feff.local/ or http://mycustomapname.local/ (if a custom AP SSID was configured in your sketch)

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/wifiConfig.png">



# Custom Access Point SSID with / without Password:

```
#include <ESP32_WiFi_Connect.h>
ESP32_WiFi_Connect espwifi;

void setup() {
  Serial.begin(115200);

  espwifi.debug(true); // Enable ESP32_WiFi_Connect debugging
  espwifi.setCustomAP("MyCustomAPName"); // Insecure Custom Access Point SSID, without password (insecure open network)
  //espwifi.setCustomAP("MyCustomAPName", "S3curePa55"); // Custom Access Point SSID, secured with password
  espwifi.begin();
}

void loop() {}
```


## Advanced features:

```
#include <ESP32_WiFi_Connect.h>
ESP32_WiFi_Connect espwifi;

void setup() {
  Serial.begin(115200);

  espwifi.debug(true); // Enable ESP32_WiFi_Connect debugging
  // espwifi.setCustomAP("MyCustomAPName"); // Insecure Custom Access Point SSID, without password (insecure open network)
  espwifi.setCustomAP("MyCustomAPName", "S3curePa55"); // Custom Access Point SSID, secured with password
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
```

# Usage:



Connect to your AP WiFi (with Password, if configured):

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_wifi_connect_with_password.png">




Once connected, go to the AP Hostname url in a browser to configure the device. The AP Hostname url will be your APName.local (example: http://easyesp-3e76feff.local/ or http://mycustomapname.local/ if a custom AP SSID was configured in your sketch):

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_wifi_connected.png">




Once on the AP Config Web Page, select or enter your WiFi SSID and Password, then save. The device will reboot and automatically connect to the configured WiFi router:

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_hostname_url_config_page.png">




Your AP SSID will also show up your WiFi Router with an IP, when connected. When connected to the same WiFi, you can still access the AP via the original APHostname.local url, or via the IP:
<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_hostname_on_wifi_router_with_ip.png">


<img width="999" height="992" alt="image" src="https://github.com/user-attachments/assets/3e292138-2760-425f-8c4d-dbaf360ba854" />
