#include <WiFi.h>
const char* ssid = "Wokwi-GUEST"; //masukan nama wifi
const char* password = ""; //masukan password wifi

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("=");
  }
}

void loop() {

}
