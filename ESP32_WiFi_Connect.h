#ifndef ESP32_WiFi_Connect_h
#define ESP32_WiFi_Connect_h

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <MD5Builder.h>
#include <vector>

struct MenuItem {
  String title;
  String url;
};

class ESP32_WiFi_Connect {
public:
  ESP32_WiFi_Connect();
  void begin();
  void replaceHeader(const String& html);
  void replaceHeader(const char* html);
  void setContent(const String& html);
  void setContent(const char* html);
  void replaceFooter(const String& html);
  void replaceFooter(const char* html);
  void pageTitle(const String& html);
  void pageTitle(const char* html);
  void setCustomAP(const String& ssid, const String& pass = "");
  void setDefaultWiFi(const String& ssid, const String& pass = "");
  void setDashboard(const String& dashboard);
  void addToMenu(const String& title, const String& url);
  void debug(bool enable = false);
  void keepAlive();
  void setWifiTimeout(uint32_t timeout_ms);
  using RouteHandler = std::function<void(AsyncWebServerRequest*)>;
  void onGet(const String& path, RouteHandler handler);
  void onPost(const String& path, RouteHandler handler);
  void sendHtml(AsyncWebServerRequest* request, const String& html = "", const int code = 200);
  void sendHtmlPage(AsyncWebServerRequest* request, const String& content = "", const String& header = "", const String& footer = "");
  bool isConnected()   { return _connection; }
  bool isConfigured()  { return _configured; }
  bool isFailed()      { return _failed; }
  String getStatus();

private:
  void _initDevice();
  bool _connectSTA();
  void _startAP();
  void _setEasySSID();
  void _loadFromNVS();
  void _saveToNVS();
  void _setSSID(const char* s);
  void _setPASS(const char* p);
  void _debugInfoln(const String& str);
  void _debugInfo(const String& str);
  String _getEffectiveHeader(const String& override = "") const;
  String _getEffectiveContent(const String& override = "") const;
  String _getEffectiveFooter(const String& override = "") const;
  String _customHeader;
  String _customContent;
  String _customFooter;
  String _pageTitle;
  char _apSSID[33] = {};
  char _apPASS[65] = {};
  char _defaultDataSSID[64] = {};
  char _defaultDataPASS[64] = {};
  bool _apCustom = false;
  String _dashboard = "/wifi";
  std::vector<MenuItem> _menuItems;
  bool _connection = false;
  bool _configured = false;
  bool _failed = false;
  bool _retry = true;
  bool _debug = false;
  uint32_t _timeout_ms = 10000;
  unsigned long wifiRetryLast = 0;
  const unsigned long wifiRetry = 60000;
};

extern const char default_header[] PROGMEM;
extern const char default_content[] PROGMEM;
extern const char default_footer[] PROGMEM;

// https://downloads.arduino.cc/libraries/logs/github.com/nedesico/ESP32_WiFi_Connect/
#endif