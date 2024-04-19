#include <WiFi.h>
const char* ssid = "Wokwi-GUEST"; //masukan nama wifi
const char* password = ""; //masukan password wifi

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);
}

void loop() {

}
