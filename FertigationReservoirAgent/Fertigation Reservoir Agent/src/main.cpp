#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <ArduinoJson.h>

const char* ssid = "LAB TI";
const char* pass = "#tiundiknas";

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
