#include <Arduino.h>
#include <MQTT.h>
#include <UrusanWiFi.h>
#include <UrusanIoT.h>
#include "secret.h"
#include <TaskScheduler.h>

void penangkapPesan(String topic, String message);
void task1DetailTugas();

UrusanWiFi urusanWiFi(ssid, pass);
UrusanIoT urusanIoT(broker, port, id, brokerUsername, brokerPassword);
Scheduler penjadwal;

Task task1(3000, TASK_FOREVER, &task1DetailTugas);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  urusanWiFi.konek();
  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);
  urusanIoT.subscribe("tld/namaorganisasi/namadivisi");

  penjadwal.init();
  penjadwal.addTask(task1);
  task1.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  urusanIoT.proses();

  if(urusanWiFi.apakahKonek() == 1 && urusanIoT.apakahKonek() == 0){
    urusanIoT.konek();
  }

  penjadwal.execute();
}

/// @brief Fungsi callback dari fungsi subscribe objek urusanIoT
/// @param topic 
/// @param message 
void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\n", topic.c_str(), message.c_str());
}

/// @brief Fungsi callback dari task1
void task1DetailTugas(){
  if(urusanIoT.apakahKonek() == 1){
    urusanIoT.publish("tld/namaorganisasi", "namadivisi");
  }
}
