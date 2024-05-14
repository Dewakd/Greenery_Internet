#include <Arduino.h>
#include <Wire.h>
#include <MQTT.h>
#include <UrusanWiFi.h>
#include <UrusanIoT.h>
#include "secret.h"
#include <TaskScheduler.h>
#include <UrusanLayar.h>
#include <ArduinoJson.h>


void penangkapPesan(String topic, String message);
void task1DetailTugas();
void task2DetailTugas();
void subscribeTopik();

UrusanWiFi urusanWiFi(ssid, pass);
UrusanIoT urusanIoT(broker, port, id, brokerUsername, brokerPassword);
UrusanLayar urusanLayar;
Scheduler penjadwal;

Task task1(3000, TASK_FOREVER, &task1DetailTugas);
Task task2(5000, TASK_FOREVER, &task2DetailTugas);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  urusanWiFi.konek();
  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);

  if(urusanIoT.apakahKonek() == 1){
    subscribeTopik();
  }

  urusanLayar.mulai();

  penjadwal.init();
  penjadwal.addTask(task1);
  penjadwal.addTask(task2);
  task1.enable();
  task2.enable();
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
    //...
    serializeJson(data, muatan);
    urusanIoT.publish("tld/namaorganisasi/namadivisi", muatan);
  
  }
}


uint8_t nomorSlider = 1;
uint8_t jumlahSlider = 4;
void task2DetailTugas(){
  if(nomorSlider == 1){
    urusanLayar.updateTemperatureAndHumidity(29, 80);
  }
  else if(nomorSlider == 2){
    urusanLayar.updateFanStatus(100, 1);
  }
  else if(nomorSlider == 3){
    urusanLayar.updateWaterReservoir(80);
  }
  else if(nomorSlider == 4){
    urusanLayar.updatePumpStatus(1);
  }   


  if(nomorSlider == jumlahSlider){
    nomorSlider = 1;
  }else{
    nomorSlider++;
  }
}