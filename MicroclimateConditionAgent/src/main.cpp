#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "secret.h"

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
void loop() {
  // put your main code here, to run repeatedly:
  myWiFi.run();
  myWs.cleanupClients();

  //Eksekusi setiap 0.1 detik sekali
  unsigned long now = millis();
  if( (now - SCHEDULER_WS_ROUTINE) >= 100 ){

    //Persiapakan data yang akan dibroadcast
    StaticJsonDocument<512> doc;
    doc["rssi"] = min(max(2 * (WiFi.RSSI() + 100), 0), 100);
    doc["ip"] = WiFi.localIP();
    doc["ssid"] = WiFi.SSID();
    doc["temperature"] = random(17, 32);
    doc["humidity"] = random(0, 100);
    doc["uptime"] = millis();
    //broadcast data ke semua klien
    sendData(doc);

    SCHEDULER_WS_ROUTINE = now;
  }

  //Eksekusi setiap 1 detik sekali
  now = millis();
  if( (now - SCHEDULER_EXECUTE_ROUTINE) >= 1000 ){
    /*setLedBrightness(0, pinLed[0], random(0, 100));
    setLedBrightness(1, pinLed[1], random(0, 100));
    setLedBrightness(2, pinLed[2], random(0, 100));*/
    
    SCHEDULER_EXECUTE_ROUTINE = now;
  }
}

// put function definitions here:
void setLedBrightness(uint8_t channel, uint8_t pinLed, uint8_t brightness){
  const int freq = 5000;
  const int resolution = 8;

  ledBrightness[channel] = brightness;

  // configure LED PWM functionalitites
  ledcSetup(channel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pinLed, channel);

  ledcWrite(channel, map(brightness, 0, 100, 0, 255));

  syncSlider(channel + 1);

  Serial.printf("LED %d is set to %d\n", channel, brightness);
}

void sendData(StaticJsonDocument<512> &doc){
  String output;
  serializeJson(doc, output);
  if(myWs.count() > 0){
    myWs.textAll(output.c_str());
  }
  //Serial.printf("Data sent: %s\n", output.c_str());
}

void syncSwitch(uint8_t switchId){
  StaticJsonDocument<512> doc;
  doc["switch"] = switchId;
  doc["state"] = ledState[switchId - 1];
  sendData(doc);
}

void syncSlider(uint8_t sliderId){
  StaticJsonDocument<512> doc;
  doc["slider"] = sliderId;
  doc["brightness"] = ledBrightness[sliderId - 1];
  sendData(doc);
}

void setSwitch(uint8_t switchId){
  if(switchId == 1){
    ledState[0] = !ledState[0];
    digitalWrite(pinLed[0], ledState[0]);
    syncSwitch(1);
    Serial.printf("Switch %d is %s\n", switchId, ledState[switchId - 1] ? "ON" : "OFF");
  }
  else if(switchId == 2){
    ledState[1] = !ledState[1];
    digitalWrite(pinLed[1], ledState[1]);
    syncSwitch(2);
    Serial.printf("Switch %d is %s\n", switchId, ledState[switchId - 1] ? "ON" : "OFF");
  }
  else if(switchId == 3){
    ledState[2] = !ledState[2];
    digitalWrite(pinLed[2], ledState[2]);
    syncSwitch(3);
    Serial.printf("Switch %d is %s\n", switchId, ledState[switchId - 1] ? "ON" : "OFF");
  }
}

void onWiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.print("WiFi connected: ");
  Serial.print(WiFi.SSID());
  Serial.print(" | Signal: ");
  Serial.print(min(max(2 * (WiFi.RSSI() + 100), 0), 100));
  Serial.println("%");
}

void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi disconnected!");
}

void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.print("Got IP Address: ");
  Serial.println(WiFi.localIP());
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    syncSwitch(1);
    syncSwitch(2);
    syncSwitch(3);

    syncSlider(1);
    syncSlider(2);
    syncSlider(3);
  } 
  else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
  } 
  else if(type == WS_EVT_ERROR){
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } 
  else if(type == WS_EVT_PONG){
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } 
  else if(type == WS_EVT_DATA){
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, data);
    if(err == DeserializationError::Ok){
      if(doc["switch"] != nullptr){
        if(doc["switch"].as<int>() == 1){
          setSwitch(1);
        }
        else if(doc["switch"].as<int>() == 2){
          setSwitch(2);
        }
        else if(doc["switch"].as<int>() == 3){
          setSwitch(3);
        }
      }
      if(doc["slider"] != nullptr){
        if(doc["slider"].as<uint8_t>() == 1){
          setLedBrightness(0, pinLed[0], doc["brightness"].as<uint8_t>());
        }
        else if(doc["slider"].as<uint8_t>() == 2){
          setLedBrightness(1, pinLed[1], doc["brightness"].as<uint8_t>());
        }
        else if(doc["slider"].as<uint8_t>() == 3){
          setLedBrightness(2, pinLed[2], doc["brightness"].as<uint8_t>());
        }
      }
    }
    else{
      Serial.println(err.c_str());
    }
  }
}