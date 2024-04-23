#include <WiFi.h>
#include <PubSubClient.h>
#include "secret.h"

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
  if(topics == "undiknas/FTI/TI/2022/IoT/Aditya/lampu"){
    if(payload[0] == 1 ){
      Serial.println("Nyalakan Lampu...");
    }
    else{
      Serial.println("Matikan Lampu...");
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Aditya123555777")) {
      Serial.println("connected");
      // Once connected, subscribe to the topic you wish to receive messages on
      client.subscribe("undiknas/FTI/TI/2022/IoT/Aditya/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

      
    }
  }

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Other loop code goes here
}
