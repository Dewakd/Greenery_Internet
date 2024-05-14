#include <Arduino.h>
#include <MQTT.h>
#include <UrusanWiFi.h>
#include <UrusanIoT.h>
#include "secret.h"
#include <TaskScheduler.h>
#include <UrusanAktuatorLingkungan.h>
#include <ArduinoJson.h>


void penangkapPesan(String topic, String message);
void task1DetailTugas();
void subscribeTopik();

UrusanWiFi urusanWiFi(ssid, pass);
UrusanIoT urusanIoT(broker, port, id, brokerUsername, brokerPassword);
UrusanAktuatorLingkungan urusanAktuatorLingkungan(32, 33);
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

  urusanAktuatorLingkungan.mulai();

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

  JsonDocument dataMasuk;
  DeserializationError galatParseJson = deserializeJson(dataMasuk, message);
  if(galatParseJson == DeserializationError::Ok){
    if(dataMasuk["perintah"] != nullptr){
      String perintah = dataMasuk["perintah"].as<String>();

      if(perintah == String("nyalakan")){
        if( dataMasuk["arah"] != nullptr && dataMasuk["kekuatan"] != nullptr){
          bool arah = dataMasuk["arah"].as<bool>();
          uint8_t kekuatan = dataMasuk["kekuatan"].as<uint8_t>();
          urusanAktuatorLingkungan.nyalakan(kekuatan, arah);
        }
      }
      else if(perintah == String("anginTopan")){
        if( dataMasuk["arah"] != nullptr){
          bool arah = dataMasuk["arah"].as<bool>();
          urusanAktuatorLingkungan.anginTopan(arah);
        }
      }
      else if(perintah == String("padamkan")){
        urusanAktuatorLingkungan.padamkan();
      }
    }
    
  }
  else{
    Serial.println("penangkapPesan: Format pesan tidak valid! Gunakan format JSON.");
  }
}

void task1DetailTugas(){
  if(urusanIoT.apakahKonek() == true){
    JsonDocument data;
    char muatan[512];

    data["arah"] = urusanAktuatorLingkungan.bacaArah();
    data["kekuatan"] = urusanAktuatorLingkungan.bacaKekuatan();

    serializeJson(data, muatan);

    urusanIoT.publish("tld/namaorganisasi/namadivisi", muatan);
  
  }
}