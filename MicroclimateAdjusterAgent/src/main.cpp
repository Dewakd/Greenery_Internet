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

  urusanWiFi.konek(); // menghubungkan wifi
  urusanIoT.konek(); // menghubungkan ke broker MQTT
  urusanIoT.penangkapPesan(penangkapPesan); // call back untuk pesan MQTT 

// Ngecek apakah terhubung ke broker MQTT dan mensubscribe topik
  if(urusanIoT.apakahKonek() == 1){ 
    subscribeTopik();
  }

// mengkonfigurasi Aktuator
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
  urusanIoT.subscribe("id/greenet/microclimateadjusteragent/setelan");
}

void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\n", topic.c_str(), message.c_str());

  JsonDocument dataMasuk;
  DeserializationError galatParseJson = deserializeJson(dataMasuk, message);
  if(galatParseJson == DeserializationError::Ok){
    if(dataMasuk["perintah"] != nullptr){
      String perintah = dataMasuk["perintah"].as<String>(); //mengambil nilai perintah jika ada

      if(perintah == String("nyalakan")){ // jika perintah nyalakan
        if( dataMasuk["arah"] != nullptr && dataMasuk["kekuatan"] != nullptr){ //jika terdapat arah dan kekuatan ada
          bool arah = dataMasuk["arah"].as<bool>(); // mengambil nilai arah
          uint8_t kekuatan = dataMasuk["kekuatan"].as<uint8_t>(); // mengambil nilai kekuatan
          urusanAktuatorLingkungan.nyalakan(kekuatan, arah); // nyalakan sesuai dengan perintah
        }
      }
      else if(perintah == String("anginTopan")){ // jika ada perintah angin topan
        if( dataMasuk["arah"] != nullptr){ // jika terdapat arah
          bool arah = dataMasuk["arah"].as<bool>(); // mengambil nilai arah
          urusanAktuatorLingkungan.anginTopan(arah); // menyalakan mode angin topan
        }
      }
      else if(perintah == String("padamkan")){ // jika ada perintah padamkan
        urusanAktuatorLingkungan.padamkan(); // memadamkan aktuator
      }
      else if(perintah == String("setArah")){
        if(dataMasuk["arah"] != nullptr){
          bool arah = dataMasuk["arah"].as<bool>();
          Serial.printf("setArah: %d \n", arah);
          urusanAktuatorLingkungan.setArah(arah);
        }
      }
      else if(perintah == String("setKekuatan")){
        if(dataMasuk["kekuatan"] != nullptr){
          uint8_t kekuatan = dataMasuk["kekuatan"].as<uint8_t>();
          Serial.printf("setKekuatan: %d \n", kekuatan);
          urusanAktuatorLingkungan.setKekuatan(kekuatan);
        }
      }
    }
    
  }
  else{
    Serial.println("penangkapPesan: Format pesan tidak valid! Gunakan format JSON."); // Mencetak pesan error jika format JSON tidak valid
  }
}

void task1DetailTugas(){
  if(urusanIoT.apakahKonek() == true){ // jika terhubung ke broker MQTT
    JsonDocument data; // membuat file json
    char muatan[512]; // buffer muatan

    data["arah"] = urusanAktuatorLingkungan.bacaArah(); // Membaca arah aktuator dan menyimpan ke JSON
    data["kekuatan"] = urusanAktuatorLingkungan.bacaKekuatan(); // Membaca kekuatan aktuator dan menyimpan ke JSON
    data["status"] = urusanAktuatorLingkungan.bacaStatus(); // Membaca status

    serializeJson(data, muatan); // Mengubah dokumen JSON menjadi string

    urusanIoT.publish("id/greenet/microclimateadjusteragent", muatan); // Mempublikasikan muatan JSON ke topik MQTT
  
  }
}