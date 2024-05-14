#ifndef UrusanWiFi_h
#define UrusanWiFi_h

#include <Arduino.h> // Include Arduino core functionalities
#include <WiFi.h>


class UrusanWiFi {
public:
  // Constructor to initialize with SSID and password
  UrusanWiFi(const char* ssid, const char* pass);
  UrusanWiFi(const char* ssid);

  // Methods provided by your UrusanWiFi library (replace with actual functions)
  void konek();
  bool apakahKonek();
  // Add other relevant methods from your UrusanWiFi library

private:
  // Private members specific to your UrusanWiFi library implementation
  const char* _ssid;
  const char* _pass;

  void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
};

#endif