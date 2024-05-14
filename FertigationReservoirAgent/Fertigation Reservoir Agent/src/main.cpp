#include <Arduino.h>
#include <MQTT.h>
#include <UrusanWiFi.h>
#include <UrusanIoT.h>
#include "secret.h"
#include <TaskScheduler.h>
#include <UrusanSensorReservoirLevel.h>
#include <ArduinoJson.h>


void penangkapPesan(String topic, String message);
void task1DetailTugas();
void subscribeTopik();

UrusanWiFi urusanWiFi(ssid, pass);
UrusanIoT urusanIoT(broker, port, id, brokerUsername, brokerPassword);
UrusanSensorReservoirLevel urusanSensorReservoirLevel(100.0, 33, 34); //tinggi reservoir, pin trigger, pin echo
Scheduler penjadwal;

Task task1(3000, TASK_FOREVER, &task1DetailTugas);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  urusanWiFi.konek();
  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);
  if(urusanIoT.apakahKonek() == 1){
    subscribeTopik();
  }

  urusanSensorReservoirLevel.mulai();

  penjadwal.init();
  penjadwal.addTask(task1);
  task1.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  urusanIoT.proses();

  if(urusanWiFi.apakahKonek() == true && urusanIoT.apakahKonek() == false){
    urusanIoT.konek();
    if(urusanIoT.apakahKonek() == 1){
      subscribeTopik();
    }
  }

  penjadwal.execute();
}

void subscribeTopik(){
  urusanIoT.subscribe("id/greenet/fertigationreservoiragent/setelan");
}

void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\n", topic.c_str(), message.c_str());
}

void task1DetailTugas(){
  if(urusanIoT.apakahKonek() == true){
    if(urusanSensorReservoirLevel.apakahSensorSiap() == true){
      float jarak = urusanSensorReservoirLevel.bacaJarak();
      float level = urusanSensorReservoirLevel.bacaLevel(jarak);

      JsonDocument data;
      char muatan[512];

      data["jarak"] = jarak;
      data["level"] = level;

      serializeJson(data, muatan);

      urusanIoT.publish("id/greenet/fertigationreservoiragent", muatan);
    }
  }
}
