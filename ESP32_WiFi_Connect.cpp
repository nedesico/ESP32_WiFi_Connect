#include "ESP32_WiFi_Connect.h"

char esp32_wifi_connect_ap_html_default[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
  <head>
    <title>EasyESP WiFi Connect</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      *, *:before, *:after {
          -webkit-box-sizing: border-box;
          -moz-box-sizing: border-box;
          box-sizing: border-box;
          position: relative;
      }
      html, body {
          height: auto;
      }
      html {
        font-family: sans-serif;
        -webkit-text-size-adjust: 100%;
        -ms-text-size-adjust: 100%;
        display: inline-block;
        text-align: left;
      }
      body {
          max-width: 1820px;
          margin: 0px auto;
          padding: 20px 10px;
          font-size: 12px;
          color: #333333;
          letter-spacing: 1.5px;
      }
      button, input[type="submit"] {
          background-color: #efefef;
          cursor: pointer;
          transition: background-color 0.1s ease;
      }
      select {
          display: inline-block;
          min-width: 120px;
          text-align: left;
          border: 1px solid #333333;
          padding: 5px 10px;
      }
      .button {
          display: inline-block;
          min-width: 120px;
          text-align: center;
          border: 1px solid #333333;
          padding: 5px 10px;
          background: #ffffff;
          color: #555555;
      }
      .button.red {
          background: #DD0000;
          color: #ffffff;
      }
      button:hover, input[type="submit"]:hover {
          background-color: #333333;
          color: #ffffff;
          transition: background-color 0.1s ease;
      }
      h4, h2 {
          margin: 0 0 10px;
      }
      h2 {
          font-size: 22px;
      }
      h4 {
          font-size: 14px;
      }
      p {
          color: #aaaaaa;
      }
      a, a:hover {
        text-decoration: none;
      }
      .wrapper {
        display: grid;
        grid-template-columns: 50% 50%;
        align-items: center;
      }
      #footer {
        padding: 20px 0;
      }
      .menu {
          display: block;
          padding: 0 0 2px;
          margin: 0 0 40px;
          border-bottom: 1px solid #aaa;
      }
      span.menu-item {
          display: inline-block;
          padding: 5px 10px;
          min-width: 60px;
          text-align: center;
          border-left: 1px solid #aaa;
      }
      .menu span.menu-item:first-child {
          border-left: none;
      }
      .row, #content {
        display: block;
        padding: 10px 5px;
      }
      input[type="text"], input[type="password"], textarea {
          display: block;
          width: 100%;
          padding: 5px 10px;
      }
      #content {
          position: relative;
          max-width: 600px;
          margin: 0 auto;
      }
      .reset-box {
        text-align: left;
      }
      ul {
         list-style: none;
         padding: 0;
         margin: 0;
      }
      li {
        padding: 15px 10px;
        border-bottom: 1px solid #ddd;
        display: grid;
        grid-template-columns: 70% 15% 15%;
        margin: 0;
      }
      .network-name {
        text-align: left;
      }
      .network-strength {
        text-align: center;
      }
      .network-encryption {
        text-align: right;
      }
      .input-field {
        display: block;
        padding: 0 0 5px;
      }
      .password-field {
        display: grid;
        grid-template-columns: 86% 14%;
      }
      #manual-scan {
        display: inline-block;
        float: right;
        position: relative;
        font-size: 18px;
        cursor: pointer;
      }
      #loading-dots {
        position: relative;
        font-size: 20px;
        padding-right: 20px;
        text-align: right;
      }
      #scan {
        padding: 10px;
        height: 300px;
        overflow-x: auto;
      }
      #toggle-password {
          display: grid;
          text-align: center;
          border: 1px solid #333333;
          border-left: none;
          align-items: center;
      }
      .pointer {
          cursor: pointer;
      }
      .pointer:hover {
        background: #EFEFEF;
      }


      @media(max-width: 620px) {
          h2 {
              font-size: 20px;
          }
          #firmware-button {
              display: none;
          }
          #content {
              display: block;
              padding: 0;
          }
          .box .columns .box {
              padding: 5px 0;
          }
          .input-box .label {
              display: block;
              width: 100%;
              padding: 0 0 5px;
          }
          .box-right {
              text-align: left;
              padding: 11px 0;
          }
          #device-wrapper {
              padding: 20px 0;
              height: 100vh;
          }
          #device-wrapper > .text-center {
              display: none;
          }
      }
    </style>
  </head>
  <body>
    <div id="content">
    %MENU%
    <h4>EasyESP WiFi Connect</h4>
    <form id="wifi-setup" method="post" action="/">
      <div class="input-field">
        <input type="text" id="ssid" name="ssid" %PREFILL_SSID% placeholder="WiFi SSID" required>
      </div>
      <div class="input-field password-field">
        <input type="password" id="pass" name="pass" %PREFILL_PASS% placeholder="WiFi Password" required><span id="toggle-password" class="pointer">Show</span>
      </div>
    </form>
    <div class="input-field">
      <button class="button" onclick="saveWiFiData();">Save Config</button>
    </div>
    <div id="save-message"></div>
    <br><br>
    <div style="border-bottom: 1px solid #ddd; padding-bottom: 10px;"><span><strong>Enter the SSID above, or select a network below</strong></span><span onclick="scan(true);" id="manual-scan">&#x27F3;</span></div>
    <div id="loading-dots">&nbsp;</div>
    <div id="scan">Scanning for WiFi networks...</div>
    %FOOTER%
    </div>
    <script>
      const togglePasswordButton = document.getElementById('toggle-password');
      const ssidField = document.getElementById('ssid');
      const passwordField = document.getElementById('pass');
      const dotElement = document.getElementById("loading-dots");
      let currentScanXhr = null;
      let autoScan = true;
      let loading = null;

      togglePasswordButton.addEventListener('click', function () {
          const type = passwordField.getAttribute('type') === 'password' ? 'text' : 'password';
          passwordField.setAttribute('type', type);
          this.textContent = type === 'password' ? 'Show' : 'Hide';
      });

      function updateSSIDInput(ssid) {
        ssidField.value = ssid;
        passwordField.focus();
        passwordField.select();
      }

      function saveWiFiData() {
          const form = document.getElementById("wifi-setup");
          const formData = new FormData(form);
          
          const xhr = new XMLHttpRequest();
          xhr.open("POST", "/updatewifi?action=1", true);
          xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

          xhr.onreadystatechange = function() {
              if(xhr.readyState === XMLHttpRequest.DONE) {
                  const msgDiv = document.getElementById("save-message");
                  msgDiv.innerHTML = "<p>" + xhr.responseText + "</p>";                  
                  setTimeout(() => msgDiv.innerHTML = "", 10000);
              }
          };

          
          xhr.send(new URLSearchParams(formData));
      }

      function factoryReset() {
          if(!confirm("Are you sure you want to reset to factory settings?")) return;
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/factoryreset", true);
          xhr.send();
          showNotice();
      }

      function showLoading() {
        let dots = 0;
        const states = [".", "..", "...", "....", "...."];
        let i = 0;
        loading = setInterval(() => {
            dotElement.innerHTML = states[i];
            i = (i + 1) % 5;
        }, 600);
      }

      function stopLoading() {
        clearInterval(loading);
        dotElement.innerHTML = "&nbsp;";
      }

      scan(false);
      setInterval(() => {
          if(autoScan) {
              scan(false);
          }
      }, 20000);

      function scan(manual) {
        if(manual) {
          if(currentXhr) {
              currentXhr.abort();
          }
          autoScan = false;
        }
        currentXhr = new XMLHttpRequest();
        currentXhr.open("GET", "/scan", true);
        showLoading();
        currentXhr.onreadystatechange = function () {
          stopLoading();
          if(currentXhr.readyState === 4) {
            if(currentXhr.status === 200) {
              try {
                const networks = JSON.parse(currentXhr.responseText);
                showScanResults(networks);
              } catch (e) {
                document.getElementById("scan").innerHTML = "<p style='color:red;'>Error parsing JSON</p>";
              }
            } else if(currentXhr.status !== 0) {
              document.getElementById("scan").innerHTML = `<p style='color:red;'>Scan failed (${currentXhr.status})</p>`;
            }
            currentXhr = null;
            if(manual) {
              autoScan = true;
            }
          }
        };

        currentXhr.onabort = function () {
          stopLoading();
          console.log("Scan request was aborted");
          currentXhr = null;
        };

        currentXhr.onerror = function () {
          stopLoading();
          document.getElementById("scan").innerHTML = "<p>Network error</p>";
          currentXhr = null;
        };

        currentXhr.send();
      }

      function showScanResults(networks) {
          var div = document.getElementById("scan");
          if(!networks || networks.length === 0) {
              div.innerHTML = "<p>No networks found</p>";
              return;
          }
          networks.sort(function(a, b) {
              return b.rssi - a.rssi;
          });
          var html = "<ul>";
          for(var i = 0; i < networks.length; i++) {
              var net = networks[i];
              var rssiLabel = "Strong";
              if(net.rssi < -60) {
                rssiLabel = "Moderate";
              }
              if(net.rssi < -80) {
                rssiLabel = "Weak";
              }              
              html += '<li class="pointer" onclick="updateSSIDInput(\''+net.ssid+'\')"><div class="network-name"><strong>'+net.ssid+'</strong></div><div class="network-strength">'+rssiLabel+'</div><div class="network-encryption">'+(net.encryption === 'secured' ? '&#x1F512;' : 'O')+'</div></li>';
          }
          html += "</ul>";
          div.innerHTML = html;
      }

      function escapeHtml(text) {
          var div = document.createElement('div');
          div.textContent = text;
          return div.innerHTML;
      }
    </script>
  </body>
</html>
)=====";

#define NVS_KEY "wifi"
#define NVS_INIT 0xFEEDBEAD
#define SSID_LEN 64
#define PASS_LEN 64

char _apSSID[33];
char _apPASS[65];

typedef struct Parameters {
  unsigned long nvsInit;
  char ssid[SSID_LEN] = "NOSSIDAVAILABLE";
  char pass[PASS_LEN] = "";
} Parameters;
Parameters data;
Preferences prefs;

const byte DNS_PORT = 53;

MD5Builder esp32MD5;
AsyncWebServer esp32Server(80);
DNSServer esp32DnsServer;

unsigned long wifiRetry = 60000;
unsigned long wifiRetryLast = 60000;

ESP32_WiFi_Connect::ESP32_WiFi_Connect() {
  _timeout_ms = 10000;
}

void ESP32_WiFi_Connect::begin() {
  _initDevice();
}

void ESP32_WiFi_Connect::_initDevice() {
  _loadFromNVS();
  if(!_apCustom || _apSSID[0] == '\0') {
    _setEasySSID();
  }

  WiFi.setHostname(_apSSID);
  WiFi.mode(WIFI_AP_STA);
  if(MDNS.begin(_apSSID)) {
    _debugInfoln("mDNS started :: http://" + String(_apSSID) + ".local/");
    MDNS.addService("http", "tcp", 80);
  }

  if(_configured && _failed && millis() - wifiRetryLast > wifiRetry) {
    wifiRetryLast = millis();
    _retry = true;
  }

  if(_retry) {
    _connectSTA();
    _retry = false;
  }
  _startAP();
}

bool ESP32_WiFi_Connect::_connectSTA() {
  if(String(data.ssid)=="NOSSIDAVAILABLE" || String(data.ssid)=="") {
    _connection = false;
    _configured = false;
    _failed = false;
    return false;
  }

  _configured = true;
  _failed = true;
  WiFi.begin(data.ssid, data.pass);

  _debugInfoln("WiFi is configured. Attempting to connect.");
  uint32_t start = millis();
  while(WiFi.status() != WL_CONNECTED && millis() - start < _timeout_ms) {
    delay(500);
    _debugInfo(".");
  }

  if(WiFi.status() == WL_CONNECTED) {
    _debugInfoln("Connected to WiFi! IP: " + String(WiFi.localIP().toString().c_str()));
    _connection = true;
    _failed = false;
    return true;
  }

  if(_failed) {
    _debugInfoln("WiFi connection failed. Restarting WiFi...");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(1000);
    WiFi.mode(WIFI_AP_STA);
    _connection = false;
    _debugInfoln("WiFi restarted with AP only enabled. Please reconfigure the WiFi SSID and try again.");
  }
  return false;
}

void ESP32_WiFi_Connect::_startAP() {
  _debugInfoln("Starting AP...");

  const char* passwordToUse = (_apPASS[0] != '\0') ? _apPASS : nullptr;

  bool apOk = WiFi.softAP(_apSSID, passwordToUse);
  if(!apOk) {
    _debugInfoln("Failed to start AP! Restarting...");
  }

  IPAddress apIP(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  _debugInfoln("AP SSID: " + String(_apSSID));
  _debugInfoln("AP Broadcasted SSID: " + String(WiFi.softAPSSID()));
  _debugInfoln("AP Password: " + String(passwordToUse ? _apPASS : "(none)"));
  _debugInfoln("AP IP:" + String(WiFi.softAPIP().toString().c_str()));

  /* DNS – answer every request with the AP IP */
  esp32DnsServer.start(DNS_PORT, "*", apIP);
  esp32Server.begin();

  _debugInfoln("Captive portal ready");

  _setHtml();

  esp32Server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->redirect(_dashboard);
  });

  esp32Server.on("/wifi", HTTP_GET, [this](AsyncWebServerRequest *request){
    String html = String(_esp32_wifi_connect_ap_html);

    html = addMenu(html);
    html = addFooter(html);

    String prefillSSID = "";
    String prefillPASS = "";

    if(String(data.ssid) != "NOSSIDAVAILABLE" && String(data.ssid)[0] != '\0') {
        prefillSSID = String(data.ssid);
        prefillSSID.replace("\"", "&quot;");
        prefillSSID = "value=\"" + prefillSSID + "\" ";

        if(data.pass[0] != '\0') {
            String pass = String(data.pass);
            pass.replace("\"", "&quot;");
            prefillPASS = "value=\"" + pass + "\" ";
        }
    }

    html.replace("%PREFILL_SSID%", prefillSSID);
    html.replace("%PREFILL_PASS%", prefillPASS);

    String deviceName = String(_apSSID);

    html.replace("EasyESP", deviceName.c_str());

    request->send(200, "text/html", html);
  });

  esp32Server.on("/updatewifi", HTTP_POST, [this](AsyncWebServerRequest *request) {
    String oldNetworkHash = _computeMD5(String(data.ssid) + String(_apSSID) + String(data.pass));
    for(int i = 0; i < request->args(); i++) {
      String key = request->argName(i);
      String val = request->arg(i);

      if(key == "ssid") _setSSID(val.c_str());
      else if(key == "pass") _setPASS(val.c_str());
    }
    _saveToNVS();

    String newNetworkHash = _computeMD5(String(data.ssid) + String(_apSSID) + String(data.pass));
    _debugInfoln("Settings saved: " + _computeMD5(oldNetworkHash) + "!=" + String(newNetworkHash));
    request->send(200, "text/html", "WiFi network saved. Restarting...");
    delay(5000);
    ESP.restart();
  });

  esp32Server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
      int n = WiFi.scanNetworks();
      String json = "[";
      for(int i = 0; i < n; ++i) {
          if(i > 0) json += ",";
          json += "{";
          String ssid = WiFi.SSID(i);
          ssid.replace("\"", "\\\"");
          json += "\"ssid\":\"" + ssid + "\",";
          json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
          if(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
              json += "\"encryption\":\"none\"";
          } else {
              json += "\"encryption\":\"secured\"";
          }
          json += "}";
      }
      json += "]";
      request->send(200, "application/json", json);
  });

  esp32Server.on("/factoryreset", HTTP_GET, [this](AsyncWebServerRequest *request) {
    prefs.begin("config", false);
    prefs.clear();
    prefs.end();
    delay(2000);
    ESP.restart();
  });

  esp32Server.on("/wifirestart", HTTP_GET, [this](AsyncWebServerRequest *request) {
    delay(2000);
    ESP.restart();
  });

  auto redirect = [this](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    response->addHeader("Pragma", "no-cache");
    response->addHeader("Expires", "-1");
    response->addHeader("Location", String("http://") + _apSSID + ".local/");
    response->setCode(302);
    request->send(response);
  };

  
  esp32Server.on("/generate_204", HTTP_ANY, redirect);                   
  esp32Server.on("/gen_204", HTTP_ANY, redirect);
  esp32Server.on("/fwlink", HTTP_ANY, redirect);                         
  esp32Server.on("/redirect", HTTP_ANY, redirect);
  esp32Server.on("/hotspot-detect.html", HTTP_ANY, redirect);            
  esp32Server.on("/connectivity-check.html", HTTP_ANY, redirect);        
  esp32Server.on("/ncsi.txt", HTTP_ANY, redirect);                       
  esp32Server.on("/success.txt", HTTP_ANY, redirect);

  
  esp32Server.onNotFound([this](AsyncWebServerRequest *request){
    request->redirect(String("http://") + _apSSID + ".local/");
  });
}

String ESP32_WiFi_Connect::_addMenu(String html) {
    if(_menuItems.size()==0) {
      String emptyMenu = "<!-- no menu -->";
      html.replace("%MENU%", emptyMenu);
      return html;
    }
    String menuHtml = "<div class=\"menu\">";
    for(size_t i = 0; i < _menuItems.size(); i++) {
        menuHtml += "<span class=\"menu-item\">";
        if((_menuItems[i].title).startsWith("<")) {
          menuHtml += _menuItems[i].title;
        } else {
          menuHtml += "<a title=\"" + _menuItems[i].title + "\" href=\"" + _menuItems[i].url + "\" class=\"menu-link\">";
          menuHtml += _menuItems[i].title;
          menuHtml += "</a>";
        }
        menuHtml += "</span>";
    }
    menuHtml += "</div>";

    html.replace("%MENU%", menuHtml);
    return html;
}

String ESP32_WiFi_Connect::addMenu(const String& html) {
    return _addMenu(html);
}

String ESP32_WiFi_Connect::_addFooter(String html) {
    String footerHtml = "<div id=\"footer\" class=\"input-field\">";
      footerHtml += "<div class=\"wrapper\">";
      footerHtml += "<div class=\"box\">";
      footerHtml += "<input class=\"button red\" type=\"submit\" value=\"Factory Reset\" onclick=\"return factoryReset();\">";
      footerHtml += "</div>";
      footerHtml += "<div class=\"box\" style=\"text-align: right;\">";
      footerHtml += "<a href=\"/wifi\">WiFi Config</a>";
      footerHtml += "</div>";
    footerHtml += "</div>";
    html.replace("%FOOTER%", footerHtml);
    return html;
}

String ESP32_WiFi_Connect::addFooter(const String& html) {
    return _addFooter(html);
}

void ESP32_WiFi_Connect::_addRoute(const String& path, WebRequestMethodComposite method, RouteHandler handler) {
    esp32Server.on(path.c_str(), method, handler);
}

void ESP32_WiFi_Connect::_setEasySSID() {
      String chipId = String((uint32_t)(ESP.getEfuseMac() >> 24), HEX);
      chipId.toUpperCase();
      strcpy(_apSSID, ("EasyESP-" + chipId).c_str());
      _apPASS[0] = '\0';  
}

void ESP32_WiFi_Connect::_saveToNVS() {
  prefs.begin("config", false);   
  prefs.putBytes(NVS_KEY, &data, sizeof(data));
  prefs.end();
  _debugInfoln("WiFi config saved");
}

void ESP32_WiFi_Connect::_loadFromNVS() {
  _debugInfoln("Load WiFi config...");

  prefs.begin("config", true);  
  size_t len = prefs.getBytesLength(NVS_KEY);
  
  if(len == sizeof(Parameters)) {
    Parameters temp;
    prefs.getBytes(NVS_KEY, &temp, sizeof(temp));
    prefs.end();

    if(temp.nvsInit == NVS_INIT) {
      data = temp;
      _debugInfoln("WiFi config loaded");
      _debugInfoln("data.ssid: " + String(data.ssid));
      _debugInfoln("data.pass: " + String(data.pass));
      return;
    }
  } else {
    prefs.end();
  }

  data.nvsInit = NVS_INIT;
  strcpy(data.ssid, "NOSSIDAVAILABLE");
  data.pass[0] = '\0';
  _debugInfoln("Init default config");
  _saveToNVS();  
}


String ESP32_WiFi_Connect::_computeMD5(String& input) {
  esp32MD5.begin();
  esp32MD5.add(input.c_str());        
  esp32MD5.calculate();
  return esp32MD5.toString();          
}

void ESP32_WiFi_Connect::_setSSID(const char* s) { 
  strncpy(data.ssid, s, SSID_LEN-1); 
  data.ssid[SSID_LEN-1] = '\0'; 
}

void ESP32_WiFi_Connect::_setPASS(const char* p) { 
  strncpy(data.pass, p, PASS_LEN-1); 
  data.pass[PASS_LEN-1] = '\0'; 
}

void ESP32_WiFi_Connect::_setHtml() {
  if(!_esp32_wifi_connect_ap_html) {
    _esp32_wifi_connect_ap_html = esp32_wifi_connect_ap_html_default;
  }
}

void ESP32_WiFi_Connect::_debugInfoln(String str) {
  if(_debug) {
    Serial.println(str);
  }
}

void ESP32_WiFi_Connect::_debugInfo(String str) {
  if(_debug) {
    Serial.print(str);
  }
}

void ESP32_WiFi_Connect::setCustomAP(String ssidS, String passS) {
    const char* ssid = ssidS.c_str();
    const char* pass = passS.c_str();

    if(ssid && ssid[0]) {
        strncpy(_apSSID, ssid, sizeof(_apSSID) - 1);
        _apSSID[sizeof(_apSSID) - 1] = '\0';
    } else {
      _setEasySSID();
    }

    if(pass && pass[0]) {
        strncpy(_apPASS, pass, sizeof(_apPASS) - 1);
        _apPASS[sizeof(_apPASS) - 1] = '\0';
    } else {
        _apPASS[0] = '\0';  
    }

    _apCustom = true;
}

void ESP32_WiFi_Connect::setCustomAPhtml(char* html) {
  _esp32_wifi_connect_ap_html = html;
}

void ESP32_WiFi_Connect::setDashboard(String dashboard) {
  if(!dashboard.startsWith("/")) {
    dashboard = "/" + dashboard;
  }
  _dashboard = dashboard;
}

bool ESP32_WiFi_Connect::isConnected() {
  return _connection;
}

bool ESP32_WiFi_Connect::isConfigured() {
  return _configured;
}

bool ESP32_WiFi_Connect::isFailed() {
  return _failed;
}

void ESP32_WiFi_Connect::keepAlive() {
  esp32DnsServer.processNextRequest();   
}

void ESP32_WiFi_Connect::setWifiTimeout(uint32_t timeout_ms) {
  _timeout_ms = timeout_ms;
}

void ESP32_WiFi_Connect::debug(bool debug = false) {
  _debug = debug;
}

String ESP32_WiFi_Connect::getStatus() {
  if(!_configured) return "wifi_not_configured";
  if(_configured && _failed) return "wifi_connection_failed";
  if(_configured && _connection) return "wifi_connected";
}