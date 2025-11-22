# ESP32_WiFi_Connect

ESP32 WiFi Connect UI. Your ESP32 will automatically launch a dedicated WiFi Access Point DNS Server, with User Interface, when WiFi isn't configured yet or unavailable. This will allow you to configure and save the WiFi credentials to the onboard flash and connect to the WiFi when in range.

# Very basic usage:

```
#include <ESP32_WiFi_Connect.h> // Include the ESP32_WiFi_Connect library

ESP32_WiFi_Connect espwifi; // Instantiate the class

void setup() {
  Serial.begin(115200); // Enable Serial monitoring (optional)

  espwifi.begin(); // Run the AP and web host

  Serial.println(espwifi.getStatus()); // optional WiFi status debugging info from ESP32_WiFi_Connect. Returns (String) wifi_not_configured / wifi_connection_failed / wifi_connected
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

  espwifi.debug(true); // Enable ESP32_WiFi_Connect Serial debugging (optional)
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

#define LED1 2 // Green LED = WiFi configured and connected
#define LED2 3 // Blue LED = WiFi configured but not conneced
#define LED3 4 // Red LED = WiFi not configured

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  espwifi.debug(true); // Enable ESP32_WiFi_Connect Serial debugging (optional)
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

void loop() {
  // Light up an LED to show the device status. Putting it in the loop provides a continual visual indicator of whether the device has an active WiFi connection.
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  if(espwifi.getStatus()=='wifi_connected') {
    digitalWrite(LED1, HIGH); // Green LED shows thet the device has an active WiFi connection
  } else if(espwifi.getStatus()=='wifi_connection_failed') {
    digitalWrite(LED2, HIGH); // Blue LED shows that the WiFi is configured, but is either not connecting or is misconfigured or is out of WiFi range
  } else if(espwifi.getStatus()=='wifi_not_configured') {
    digitalWrite(LED3, HIGH); // Red LED shows that the device has not been configured
  }

  delay(2000); // Check the device connection status every 2 seconds
}
```

# Usage:



Connect to your AP WiFi (with Password, if configured):

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_wifi_connect_with_password.png">




Once connected, go to the AP Hostname url in a browser to configure the device. The AP Hostname url will be your APName.local (example: http://easyesp-3e76feff.local/ or http://mycustomapname.local/ if a custom AP SSID was configured in your sketch):

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_wifi_connected.png">




Once on the AP Config Web Page, select or enter your WiFi SSID and Password, then save. The device will reboot and automatically connect to the configured WiFi router:

<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_hostname_url_config_page.png">




Your AP SSID will also show up in your WiFi Router with it's IP, when connected. You can access the web host via the original APHostname.local url or the IP, when your phone or computer is connected to the same WiFi network:
<img src="https://raw.githubusercontent.com/nedesico/ESP32_WiFi_Connect/refs/heads/main/custom_ap_hostname_on_wifi_router_with_ip.png">


<img width="999" height="992" alt="image" src="https://github.com/user-attachments/assets/3e292138-2760-425f-8c4d-dbaf360ba854" />
