#include "ESP32_WiFi_Connect.h"

const char default_header[] PROGMEM = R"==hc==(
<!DOCTYPE HTML><html>
  <head>
    <title>%EasyESP% / %PAGE_TITLE%</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      *, *:before, *:after { -webkit-box-sizing: border-box; -moz-box-sizing: border-box; box-sizing: border-box; position: relative; }
      html, body { height: auto; }
      html { font-family: sans-serif; -webkit-text-size-adjust: 100%; -ms-text-size-adjust: 100%; display: inline-block; text-align: left; }
      body { max-width: 1820px; margin: 0px auto; padding: 20px 10px; font-size: 12px; color: #333333; letter-spacing: 1.5px; }
      button, input[type="submit"] { background-color: #efefef; cursor: pointer; transition: background-color 0.1s ease; }
      select { display: inline-block; min-width: 120px; text-align: left; border: 1px solid #333333; padding: 5px 10px; }
      .button { display: inline-block; min-width: 120px; text-align: center; border: 1px solid #333333; padding: 5px 10px; background: #ffffff; color: #555555; }
      .button.red { background: #DD0000; color: #ffffff; }
      button:hover, input[type="submit"]:hover { background-color: #333333; color: #ffffff; transition: background-color 0.1s ease; }
      h4, h2 { margin: 0 0 10px; }
      h2 { font-size: 22px; }
      h4 { font-size: 14px; }
      p { color: #aaaaaa; }
      a, a:hover { text-decoration: none; }
      .wrapper { display: grid; grid-template-columns: 50% 50%; align-items: center; }
      #footer { padding: 20px 0; }
      .menu { display: block; padding: 0 0 2px; margin: 0 0 40px; border-bottom: 1px solid #aaa; }
      span.menu-item { display: inline-block; padding: 5px 10px; min-width: 60px; text-align: center; border-left: 1px solid #aaa; }
      .menu span.menu-item:first-child { border-left: none; }
      input[type="text"], input[type="password"], textarea { display: block; width: 100%; padding: 5px 10px; }
      #content { position: relative; max-width: 600px; margin: 0 auto; }
      .input-field { padding: 0 0 10px; }
      ul { list-style: none; padding: 0; margin: 0; }
      li { padding: 15px 10px; border-bottom: 1px solid #ddd; display: grid; grid-template-columns: 70% 15% 15%; margin: 0; }
      .password-field { display: grid; grid-template-columns: 86% 14%; }
      #toggle-password { display: grid; text-align: center; border: 1px solid #333333; border-left: none; align-items: center; }
      .pointer { cursor: pointer; }
      .pointer:hover { background: #EFEFEF; }
      div#scan { display: block; height: 300px; overflow-y: auto; }
      #manual-scan { display: inline-block; float: right; font-size: 14px; line-height: 14px; cursor: pointer; }
      @media(max-width: 620px) { h2 { font-size: 20px; } #content { padding: 0; } }
      .clearfix { clear:both; }
    </style>
  </head>
  <body>
    <div id="content">
    %MENU%
    <h4>%EasyESP% / %PAGE_TITLE%</h4>
    <div id="main-content">
)==hc==";

const char default_content[] PROGMEM = R"=====(
    <form id="wifi-setup" method="post" action="/">
      <div class="input-field">
        <input type="text" id="ssid" name="ssid" value="%PREFILL_SSID%" placeholder="WiFi SSID" required>
      </div>
      <div class="input-field password-field">
        <input type="password" id="pass" name="pass" value="%PREFILL_PASS%" placeholder="WiFi Password" required><span id="toggle-password" class="pointer">Show</span>
      </div>
    </form>
    <div class="input-field clearfix">
      <button class="button" onclick="saveWiFiData();">Save Config</button> <span style="display: inline-block; float: right;"><a href="/status">Device Status</a></span>
    </div>
    <div id="save-message"></div>
    <br><br>
    <div style="border-bottom: 1px solid #ddd; padding-bottom: 10px;"><span><strong>Enter the SSID above, or select a network below</strong></span><span onclick="scan(true);" id="manual-scan">&#x27F3;</span></div>
    <div id="loading-dots">&nbsp;</div>
    <div id="scan">Scanning for WiFi networks...</div>
    <script>
      const togglePasswordButton = document.getElementById('toggle-password');
      const ssidField = document.getElementById('ssid');
      const passwordField = document.getElementById('pass');
      const dotElement = document.getElementById("loading-dots");
      let currentXhr = null;
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
      }

      function showLoading() {
        let i = 0;
        loading = setInterval(() => {
            dotElement.innerHTML = ".".repeat((i++ % 4) + 1);
        }, 600);
      }

      function stopLoading() {
        clearInterval(loading);
        dotElement.innerHTML = "&nbsp;";
      }

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

      scan(false);
      setInterval(() => { if(autoScan) scan(false); }, 20000);

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

    </script>
  </div>
)=====";

const char default_footer[] PROGMEM = R"==fc==(
    <div id="footer" class="input-field">
      <div class="wrapper">
        <div class="box"><input class="button red" type="submit" value="Factory Reset" onclick="return factoryReset();"></div>
        <div class="box" style="text-align: right;"><a href="/wifi">WiFi Config</a></div>
      </div>
    </div>
  </div>
  </body>
</html>
)==fc==";

#define NVS_KEY "wifi"
#define NVS_INIT 0xFEEDBEAD
#define SSID_LEN 64
#define PASS_LEN 64

struct Parameters {
  unsigned long nvsInit = NVS_INIT;
  char ssid[SSID_LEN] = "NOSSIDAVAILABLE";
  char pass[PASS_LEN] = "";
} data;

Preferences prefs;
MD5Builder esp32MD5;
AsyncWebServer esp32Server(80);
DNSServer esp32DnsServer;

ESP32_WiFi_Connect::ESP32_WiFi_Connect() {
  _timeout_ms = 10000;
}

void ESP32_WiFi_Connect::replaceHeader(const String& h) { _customHeader = h; }
void ESP32_WiFi_Connect::replaceHeader(const char* h)   { _customHeader = h; }
void ESP32_WiFi_Connect::setContent(const String& c){ _customContent = c; }
void ESP32_WiFi_Connect::setContent(const char* c) { _customContent = c; }
void ESP32_WiFi_Connect::replaceFooter(const String& f) { _customFooter = f; }
void ESP32_WiFi_Connect::replaceFooter(const char* f) { _customFooter = f; }
void ESP32_WiFi_Connect::pageTitle(const String& f) { _pageTitle = f; }
void ESP32_WiFi_Connect::pageTitle(const char* f)    { _pageTitle = f; }

String ESP32_WiFi_Connect::_getEffectiveHeader(const String& o) const  { return o.length() ? o : (_customHeader.length() ? _customHeader : FPSTR(default_header)); }
String ESP32_WiFi_Connect::_getEffectiveContent(const String& o) const { return o.length() ? o : (_customContent.length() ? _customContent : FPSTR(default_content)); }
String ESP32_WiFi_Connect::_getEffectiveFooter(const String& o) const  { return o.length() ? o : (_customFooter.length() ? _customFooter : FPSTR(default_footer)); }

void ESP32_WiFi_Connect::sendHtml(AsyncWebServerRequest* request,
                                      const String& html,
                                      const int code) {
  request->send(code, "text/html", html);
}

void ESP32_WiFi_Connect::sendHtmlPage(AsyncWebServerRequest* request,
                                      const String& content,
                                      const String& header,
                                      const String& footer) {
  String html = _getEffectiveHeader(header) + _getEffectiveContent(content) + _getEffectiveFooter(footer);

  html.replace("%EasyESP%", _apSSID);
  html.replace("%PAGE_TITLE%", _pageTitle);

  String menuHtml = _menuItems.empty() ? "<!-- no menu -->" : "<div class=\"menu\">";
  for (auto& item : _menuItems) {
    menuHtml += "<span class=\"menu-item\">";
    if (item.title.startsWith("<")) menuHtml += item.title;
    else menuHtml += "<a href=\"" + item.url + "\">" + item.title + "</a>";
    menuHtml += "</span>";
  }
  if (!_menuItems.empty()) menuHtml += "</div>";
  html.replace("%MENU%", menuHtml);

  if (request->url() == "/wifi") {
    String s = (String(data.ssid) != "NOSSIDAVAILABLE" && data.ssid[0]) ? String(data.ssid) : "";
    String p = data.pass[0] ? String(data.pass) : "";
    html.replace("%PREFILL_SSID%", s);
    html.replace("%PREFILL_PASS%", p);
  }

  request->send(200, "text/html", html);
}

void ESP32_WiFi_Connect::begin() { _initDevice(); }

void ESP32_WiFi_Connect::_initDevice() {
  _loadFromNVS();
  if (!_apCustom || _apSSID[0] == '\0') _setEasySSID();

  WiFi.setHostname(_apSSID);
  WiFi.mode(WIFI_AP_STA);
  if (MDNS.begin(_apSSID)) {
    _debugInfoln("mDNS started :: http://" + String(_apSSID) + ".local/");
    MDNS.addService("http", "tcp", 80);
  }

  if (_configured && _failed && millis() - wifiRetryLast > wifiRetry) {
    wifiRetryLast = millis();
    _retry = true;
  }

  _debugInfoln("_retry value: " + String(_retry));

  if (_retry) { _connectSTA(); _retry = false; }
  _startAP();
}

bool ESP32_WiFi_Connect::_connectSTA() {
  if (String(data.ssid) == "NOSSIDAVAILABLE" || String(data.ssid) == "") {
    _debugInfoln("data.ssid not set");
    _connection = _configured = _failed = false;
    return false;
  }
  _debugInfoln("data.ssid is set");

  _configured = true; _failed = true;
  WiFi.begin(data.ssid, data.pass);
  _debugInfoln("Connecting to WiFi...");

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < _timeout_ms) {
    delay(500);
    _debugInfo(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    _debugInfoln("\nConnected! IP: " + WiFi.localIP().toString());
    _connection = true; _failed = false;
    return true;
  }

  _debugInfoln("\nConnection failed");
  _connection = false;
  return false;
}

void ESP32_WiFi_Connect::_startAP() {
  _debugInfoln("Starting AP...");

  const char* pass = (_apPASS[0] != '\0') ? _apPASS : nullptr;
  WiFi.softAP(_apSSID, pass);
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));

  esp32DnsServer.start(53, "*", IPAddress(192,168,4,1));
  esp32Server.begin();

  esp32Server.on("/", HTTP_GET, [this](AsyncWebServerRequest* r) { r->redirect(_dashboard); });
  esp32Server.on("/wifi", HTTP_GET, [this](AsyncWebServerRequest* r) { pageTitle("WiFi Connect"); sendHtmlPage(r); });

  esp32Server.on("/updatewifi", HTTP_POST, [this](AsyncWebServerRequest* r) {
    for (int i = 0; i < r->args(); i++) {
      if (r->argName(i) == "ssid") _setSSID(r->arg(i).c_str());
      if (r->argName(i) == "pass") _setPASS(r->arg(i).c_str());
    }
    _saveToNVS();
    r->send(200, "text/html", "WiFi saved. Restarting...");
    delay(5000);
    ESP.restart();
  });

  esp32Server.on("/scan", HTTP_GET, [](AsyncWebServerRequest* r) {
    int n = WiFi.scanNetworks();
    String json = "[";
    for (int i = 0; i < n; ++i) {
      if (i) json += ",";
      String ssid = WiFi.SSID(i); ssid.replace("\"", "\\\"");
      json += "{\"ssid\":\"" + ssid + "\",\"rssi\":" + WiFi.RSSI(i) +
              ",\"encryption\":\"" + (WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "none" : "secured") + "\"}";
    }
    json += "]";
    r->send(200, "application/json", json);
  });

  esp32Server.on("/factoryreset", HTTP_GET, [this](AsyncWebServerRequest*) {
    prefs.begin("config", false); prefs.clear(); prefs.end();
    delay(2000); ESP.restart();
  });

  auto redir = [this](AsyncWebServerRequest* r) {
    r->redirect("http://" + String(_apSSID) + ".local/");
  };
  esp32Server.on("/generate_204", HTTP_ANY, redir);
  esp32Server.on("/gen_204", HTTP_ANY, redir);
  esp32Server.on("/fwlink", HTTP_ANY, redir);
  esp32Server.on("/hotspot-detect.html", HTTP_ANY, redir);
  esp32Server.on("/connectivity-check.html", HTTP_ANY, redir);
  esp32Server.onNotFound(redir);
}

void ESP32_WiFi_Connect::setCustomAP(const String& s, const String& p) {
  strncpy(_apSSID, s.c_str(), 32); _apSSID[32] = 0;
  strncpy(_apPASS, p.c_str(), 64); _apPASS[64] = 0;
  _apCustom = true;
}

void ESP32_WiFi_Connect::setDefaultWiFi(const String& s, const String& p) {
  strncpy(_defaultDataSSID, s.c_str(), 64); _defaultDataSSID[64] = '\0';
  strncpy(_defaultDataPASS, p.c_str(), 64); _defaultDataPASS[64] = '\0';
}

void ESP32_WiFi_Connect::setDashboard(const String& d) { _dashboard = d.startsWith("/") ? d : "/" + d; }
void ESP32_WiFi_Connect::addToMenu(const String& t, const String& u) { _menuItems.push_back({t, u}); }
void ESP32_WiFi_Connect::debug(bool e) { _debug = e; if(e) Serial.println("ESP32_WiFi_Connect degugging enabled"); }
void ESP32_WiFi_Connect::keepAlive() { esp32DnsServer.processNextRequest(); }
void ESP32_WiFi_Connect::setWifiTimeout(uint32_t t) { _timeout_ms = t; }
void ESP32_WiFi_Connect::onGet(const String& p, RouteHandler h) { esp32Server.on(p.c_str(), HTTP_GET, h); }
void ESP32_WiFi_Connect::onPost(const String& p, RouteHandler h) { esp32Server.on(p.c_str(), HTTP_POST, h); }

void ESP32_WiFi_Connect::_setEasySSID() {
  String chipId = String((uint32_t)(ESP.getEfuseMac() >> 24), HEX); chipId.toUpperCase();
  snprintf(_apSSID, sizeof(_apSSID), "EasyESP-%s", chipId.c_str());
  _apPASS[0] = '\0';
}

void ESP32_WiFi_Connect::_loadFromNVS() {
  prefs.begin("config", true);
  if (prefs.getBytesLength(NVS_KEY) == sizeof(data)) {
    prefs.getBytes(NVS_KEY, &data, sizeof(data));
    if (data.nvsInit != NVS_INIT) { data = Parameters(); _saveToNVS(); }
    _debugInfoln("data.ssid: " + String(data.ssid));
    _debugInfoln("data.pass: " + String(data.pass));
  } else {
    data = Parameters();
    if(String(_defaultDataSSID)) {
      strncpy(data.ssid, _defaultDataSSID, 64);
      strncpy(data.pass, _defaultDataPASS, 64);
      _debugInfoln("No data.ssid found");
      _debugInfoln("_defaultDataSSID found in sketch: " + String(data.pass));
      _debugInfoln("Saving _defaultDataSSID to data.ssid");
    }
    _saveToNVS();
  }
  prefs.end();
}

void ESP32_WiFi_Connect::_saveToNVS() {
  prefs.begin("config", false);
  prefs.putBytes(NVS_KEY, &data, sizeof(data));
  prefs.end();
}

void ESP32_WiFi_Connect::_setSSID(const char* s) { strncpy(data.ssid, s, SSID_LEN-1); data.ssid[SSID_LEN-1] = '\0'; }
void ESP32_WiFi_Connect::_setPASS(const char* p) { strncpy(data.pass, p, PASS_LEN-1); data.pass[PASS_LEN-1] = '\0'; }

void ESP32_WiFi_Connect::_debugInfoln(const String& s) { if(_debug) { Serial.println(s); } }
void ESP32_WiFi_Connect::_debugInfo(const String& s)  { if(_debug) Serial.print(s); }

String ESP32_WiFi_Connect::getStatus() {
  if (!_configured) return "wifi_not_configured";
  if (_failed) return "wifi_connection_failed";
  return "wifi_connected";
}