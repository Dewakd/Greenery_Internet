#ifndef UrusanWiFi_h
#define UrusanWiFi_h

#include <Arduino.h> // Sertakan senjata Arduino core
#include <WiFi.h> // Sertakan senjata WiFi

/// @brief Kelas untuk menangani segala urusan WiFi.
class UrusanWiFi {
public:
  /// @brief Inisiator kelas UrusanWiFi
  /// @param ssid 
  /// @param pass 
  UrusanWiFi(const char* ssid, const char* pass);
  UrusanWiFi(const char* ssid);

  void konek();
  bool apakahKonek();

private:

  const char* _ssid;
  const char* _pass;

  void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
};

#endif