#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MQTT.h>
#include <UrusanWiFi.h>
#include <UrusanIoT.h>
#include "secret.h"
#include <TaskScheduler.h>
#include <UrusanSensorLingkungan.h>
#include <ArduinoJson.h>


void penangkapPesan(String topic, String message);
void task1DetailTugas();
void subscribeTopik();

UrusanWiFi urusanWiFi(ssid, pass);
UrusanIoT urusanIoT(broker, port, id, brokerUsername, brokerPassword);
UrusanSensorLingkungan urusanSensorLingkungan;
Scheduler penjadwal;

Task task1(3000, TASK_FOREVER, &task1DetailTugas);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  urusanWiFi.konek();
  while(urusanWiFi.apakahKonek() == 0){
    Serial.print(".");
    delay(1000);
  }
  urusanIoT.konek();
  while(urusanIoT.apakahKonek() == 0){
    Serial.print(".");
    delay(1000);
  }
  
  urusanIoT.penangkapPesan(penangkapPesan);
  if(urusanIoT.apakahKonek() == 1){
    subscribeTopik();
  }

  urusanSensorLingkungan.mulai();

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
  urusanIoT.subscribe("tld/namaorganisasi/namadivisi/setelan");
}

void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\n", topic.c_str(), message.c_str());
}

void task1DetailTugas(){
  if(urusanIoT.apakahKonek() == true){
    if(urusanSensorLingkungan.apakahSensorSiap() == true){
      float suhu = urusanSensorLingkungan.bacaSuhu();
      float kelembapan = urusanSensorLingkungan.bacaKelembapan();

      JsonDocument data;
      char muatan[512];

      data["suhu"] = suhu;
      data["kelembapan"] = kelembapan;

      serializeJson(data, muatan);

      urusanIoT.publish("tld/namaorganisasi/namadivisi", muatan);
    }
  }
}