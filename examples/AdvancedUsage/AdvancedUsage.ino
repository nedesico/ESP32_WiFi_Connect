#include <ESP32_WiFi_Connect.h>

const char* apSSID = "MyCustomAPName"; // Use a custom AP SSID - No spaces or special characters
const char* apPASS = "MySup3r5ecurePass@99"; // Use a password to access the AP (recommended). Min 8 characters (secured), or set to "" for an open network (insecure)

// Change the default WiFi config design (Not recommended)
/*
char custom_ap_html[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
  <head>
  </head>
  <body>
    <div id="content">
    <h4>CUSTOM HTML HERE</h4>
  </body>
</html>
)=====";
*/


ESP32_WiFi_Connect espwifi; // Initialize the ESP32 WiFi Connect System
void setup() {
  Serial.begin(115200);

  espwifi.setCustomAP(apSSID, apPASS); // Set your custom AP SSID and password
  //espwifi.setCustomAPhtml(custom_ap_html); // Update the default AP html
  //espwifi.setDashboard("/customurl"); // Set the AP dashboard url. Defaults to the WiFi Config page at /wifi
  espwifi.debug(true); // Enable debugging
  espwifi.begin(); // Begin the ESP32 WiFi Connect System. All AP config settings should be set before this. All custom webpages should be set after this 

  espwifi.addToMenu("Customurl 1", "/customurl1"); // Add a url name and link to the built-in menu
  // Add a custom HTTP_GET page with the built-in ESP32 WiFi Connect quick add function.
  // espwifi.onPost("/customurl1", [](AsyncWebServerRequest *request) is also available for HTTP_POST requests
  espwifi.onGet("/customurl1", [](AsyncWebServerRequest *request) {
    String html = "%MENU%<h1>Hello World 1!</h1>%FOOTER%"; // %MENU% and %FOOTER% will be replaced by the built-in ESP32 WiFi Connect menu and footer 
    html = espwifi.addMenu(html); // Add the menu to your html code
    html = espwifi.addFooter(html); // Add the footer to your html code
    request->send(200, "text/html", html); // Send AsyncWebServer request
  });

  espwifi.addToMenu("Customurl 2", "/customurl2");
  // Add a custom HTTP_GET or HTTP_POST page using the original AsyncWebServer method.
  espwifi.on("/customurl2", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html =  "%MENU%";
           html += "<h1>Hello World 2!</h1>";
           html += "%FOOTER%";
    html = espwifi.addMenu(html);
    html = espwifi.addFooter(html);
    request->send(200, "text/html", html);
  });

  Serial.println(espwifi.getStatus()); // Get the WiFi status of the device.
  // wifi_not_configured, means the WiFi SSID hasn't been configured yet
  // wifi_connection_failed, means the WiFi SSID has been configured, but it can't connect.
  // - Check your settings, or whether the WiFi is out of range. Alternatively factory reset or format flash memory and reconfigure the device.
  // wifi_connected, means the WiFi SSID has been configured and is connected
}

void loop() {
  espwifi.keepAlive(); // Keeping the DNS server alive allows you to access the AP by connecting to the AP SSID from your phone / computer
}
