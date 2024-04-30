#include "UrusanWiFi.h"

UrusanWiFi::UrusanWiFi(const char* ssid, const char* pass) : _ssid(ssid), _pass(pass) {}
UrusanWiFi::UrusanWiFi(const char* ssid) : _ssid(ssid) {}

/// @brief Fungsi untuk memulai konek ke WiFi
void UrusanWiFi::konek() {
  Serial.printf("UrusanWiFi: Mencoba konek ke %s...\n", _ssid);
  
  if(_pass != nullptr){
    WiFi.begin(_ssid, _pass);
  }else
  {
    WiFi.begin(_ssid);
  }

  WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info){
    this->onWiFiEvent(event, info);
  });

  unsigned long timer = millis();
  while(WiFi.status() != WL_CONNECTED){
    delay(100);

    if(millis() - timer > 10000){break;}
  }
}

/// @brief Cek apakah WiFinya konek.
/// @return 1 jika konek, 0 jika putus
bool UrusanWiFi::apakahKonek() {
  if (WiFi.status() == WL_CONNECTED){
    return 1;
  }else{
    return 0;
  }

}

/// @brief Fungsi private untuk menangani event WiFi
/// @param event 
/// @param info 
void UrusanWiFi::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  if(event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED){
    Serial.printf("UrusanWiFi: Koneksi dengan %s terputus!\n", info.wifi_sta_connected.ssid);
  }
  else if(event == ARDUINO_EVENT_WIFI_STA_CONNECTED){
    Serial.printf("UrusanWiFi: Berhasil terhubung dengan %s.\n", info.wifi_sta_connected.ssid);
  }
  else if(event == ARDUINO_EVENT_WIFI_STA_GOT_IP){
    Serial.printf("UrusanWiFi: Alamat IP %s.\n", WiFi.localIP().toString().c_str());
  }
}