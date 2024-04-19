#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "UDAWA";
const char* password = "defaultkey";
const char* mqtt_server = "broker.hivemq.com"; // ubah address MQTT broker

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
