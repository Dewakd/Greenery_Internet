#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "UDAWA";
const char* password = "defaultkey";
const char* mqtt_server = "broker.hivemq.com"; // Change this to the address of your MQTT broker

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
