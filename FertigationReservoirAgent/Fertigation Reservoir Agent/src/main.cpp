#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "LittleFS.h"


WiFiMulti myWiFi;
AsyncWebServer myWeb(80);
AsyncWebSocket myWs("/ws");

// put function declarations here:
int myFunction(int, int);

void onWiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
void setSwitch(uint8_t switchId);
void sendData(StaticJsonDocument<512> &doc);
void setLedBrightness(uint8_t channel, uint8_t pinLed, uint8_t brightness);
void syncSlider(uint8_t sliderId);

uint8_t pinLed[3] = {25, 33, 32}; // hijau, biru, merah
bool ledState[3] = {false, false, false};
uint8_t ledBrightness[3] = {0, 0, 0};


uint8_t pinLed[3] = {25, 33, 32}; // hijau, biru, merah
bool ledState[3] = {false, false, false};
uint8_t ledBrightness[3] = {0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  for(int i = 0; i < 3; i++){
    pinMode(pinLed[i], OUTPUT);
  }

  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.onEvent(onWiFiConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(onWiFiDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(onWiFiGotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  myWiFi.addAP(ssid, pass);
  
  /*myWeb.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", "Hello, world");
  });*/

  myWeb.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");

  while (myWiFi.run() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }

  myWeb.begin();
  myWs.onEvent(onWsEvent);
  myWeb.addHandler(&myWs);
}

unsigned long SCHEDULER_WS_ROUTINE = 0;
unsigned long SCHEDULER_EXECUTE_ROUTINE = 0;