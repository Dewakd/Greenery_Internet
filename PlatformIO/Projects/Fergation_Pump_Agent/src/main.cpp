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
