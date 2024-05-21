#include "UrusanIoT.h"

/// @brief Inisiator untuk membuat instan dari kelas UrusanIoT.
/// @param broker 
/// @param port 
/// @param clientId 
/// @param brokerUsername 
/// @param brokerPassword 
UrusanIoT::UrusanIoT(const char* broker, const int port, const char* clientId, 
    const char* brokerUsername, const char* brokerPassword) : _broker(broker),
     _port(port), _clientId(clientId), _brokerUsername(brokerUsername), _brokerPassword(brokerPassword) {
    
}

/// @brief Konek ke MQTT Broker
void UrusanIoT::konek(){
    if(WiFi.status() != WL_CONNECTED){ // Memeriksa status koneksi wifi
      Serial.printf("UrusanIoT: Tidak ada koneksi!\n"); // Mencetak pesan jika tidak ada koneksi WiFi
      delay(1000);
      return;
    }
    Serial.printf("UrusanIoT: Mencoba untuk konek ke %s:%d\n", _broker, _port); // Mencetak pesan percobaan koneksi ke broker
    client.begin(_broker, _port, net);
    bool status = client.connect(_clientId, _brokerUsername, _brokerPassword); // Mencoba menghubungkan ke broker dengan ID klien, username, dan password

    unsigned long timer = millis();
    while(!status){
      delay(100);
      if(millis() - timer > 10000){break;}
    }

    if(status){
        Serial.printf("UrusanIoT: Berhasil terhubung ke %s:%d\n", _broker, _port);
    }else{
        Serial.printf("UrusanIoT: Gagal terhubung ke %s:%d!\n", _broker, _port);
    }
}

/// @brief Cek apakah IoTnya konek.
/// @return 1 jika konek, 0 jika putus
bool UrusanIoT::apakahKonek() { // Memeriksa apakah klien terhubung ke broker
  if (client.connected()){
    return 1; // jika terhubung
  }
  else {
    return 0; // jika tidak terhubung
  }
}

/// @brief Panggil fungsi ini agar mesin IoT berjalan.
void UrusanIoT::proses(){
  client.loop();
  delay(10);
}

/// @brief Fungsi penangkap pesan masuk dari IoT
/// @param penangkapPesan
void UrusanIoT::penangkapPesan(MQTTClientCallbackSimpleFunction penangkapPesan){
  client.onMessage(penangkapPesan);
}

/// @brief Kirim pesan ke broker
/// @param topic 
/// @param message 
void UrusanIoT::publish(String topic, String message){
  Serial.printf("UrusanIoT: Mempublish pesan: %s, ke topik: %s.\n", message.c_str(), topic.c_str()); // Mencetak pesan yang akan dipublikasikan dan topiknya
  client.publish(topic.c_str(), message.c_str()); // Mempublikasikan pesan ke topik yang ditentukan
}

/// @brief Subscribe topik dari broker
/// @param topic 
void UrusanIoT::subscribe(String topic){
  Serial.printf("UrusanIoT: Mensubsribe topik: %s.\n", topic.c_str()); // Mencetak pesan topik yang akan disubscribe
  client.subscribe(topic.c_str()); // Mensubscribe ke topik yang ditentukan
}