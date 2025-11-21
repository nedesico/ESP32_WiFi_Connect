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

class AsyncWebServer;                  
typedef std::function<void(AsyncWebServerRequest*)> RouteHandler;  

class ESP32_WiFi_Connect {
  public:
    ESP32_WiFi_Connect();
    void        begin();
    void        keepAlive();
    bool        isConnected();
    bool        isConfigured();
    bool        isFailed();
    void        setCustomAPhtml(char* html);
    void        setCustomAP(String ssid, String pass);
    void        setDashboard(String dashboard);
    void        setWifiTimeout(uint32_t timeout_ms);
    void        debug(bool debug);
    String      getStatus();
    char        _apSSID[33] = "EasyESP_Config";   
    char        _apPASS[65] = "";               
    bool        _apCustom = false;
    void        on(const String& path, RouteHandler handler) { _addRoute(path, HTTP_ANY, handler); }
    void        on(const String& path, WebRequestMethodComposite method, RouteHandler handler) { _addRoute(path, method, handler); }
    void        onGet(const String& path, RouteHandler handler)    { _addRoute(path, HTTP_GET, handler); }
    void        onPost(const String& path, RouteHandler handler)   { _addRoute(path, HTTP_POST, handler); }
    void        addToMenu(const String& title, const String& url) { _addMenuItem(title, url); }
    String      addMenu(const String& html);
    String      addFooter(const String& html);


  private:
    float       _timeout_ms; 
    String      _dashboard = "/wifi";
    bool        _connection = false;
    bool        _configured = false;
    bool        _failed = true;
    bool        _retry = true;
    char*        _esp32_wifi_connect_ap_html;
    bool        _debug = false;
    void        _setSSID(const char* s); 
    void        _setPASS(const char* p); 
    void        _saveToNVS(); 
    void        _loadFromNVS(); 
    void        _startAP(); 
    void        _setDefaultHtml();
    bool        _connectSTA(); 
    String      _computeMD5(String& input);
    void        _setHtml();
    void        _initDevice();
    void        _setEasySSID();
    void        _debugInfo(String str);
    void        _debugInfoln(String str);
    void        _addRoute(const String& path, WebRequestMethodComposite method, RouteHandler handler);
    String      _addMenu(String html);
    String      _addFooter(String html);
    void        _addMenuItem(const String& title, const String& url) { _menuItems.push_back({title, url}); }
    struct        MenuItem { String title; String url; }; std::vector<MenuItem> _menuItems;

};

// https://downloads.arduino.cc/libraries/logs/github.com/nedesico/ESP32_WiFi_Connect/
#endif