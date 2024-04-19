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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String topics = String(topic);
  if(topics == "undiknas/FTI/TI/2022/IoT/Aditya/lampu") {
    if(payload[0] == 1 ){
      Serial.println("Nyalakan Lampu...");
    }
    else{
      Serial.println("Matikan Lampu...");
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
