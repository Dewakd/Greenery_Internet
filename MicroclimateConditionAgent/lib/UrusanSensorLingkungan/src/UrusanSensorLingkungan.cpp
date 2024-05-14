#include "UrusanSensorLingkungan.h"

UrusanSensorLingkungan::UrusanSensorLingkungan() : sht31() {}

/// @brief Fungsi untuk menginisialisasi sensor lingkungan
/// @return true jika sensor siap, false jika sensor bermasalah
bool UrusanSensorLingkungan::mulai(){
    if(sht31.begin(0x44)){
        sensorSiap = true;
        Serial.printf("UrusanSensorLingkungan: Modul sensor lingkungan siap digunakan.\n");
        return true;
    }else{
        sensorSiap = false;
        Serial.printf("UrusanSensorLingkungan: Gagal menemukan dan mengaktifkan modul sensor lingkungan!\n");
        return false;
    }
}

/// @brief Fungsi untuk membaca suhu lingkungan
/// @return float dalam format celsius
float UrusanSensorLingkungan::bacaSuhu(){
    if(sensorSiap){
        return sht31.readTemperature();
    }else{
        return 0.0;
    }
}

/// @brief Fungsi untuk membaca kelembapan lingkungan
/// @return float dalam format % relatif
float UrusanSensorLingkungan::bacaKelembapan(){
    if(sensorSiap){
        return sht31.readHumidity();
    }else{
        return 0.0;
    }
}

/// @brief Fungsi untuk membaca status kesiapan sensor lingkungan
/// @return true jika sensor siap, false jika tidak
bool UrusanSensorLingkungan::apakahSensorSiap(){
    if(sensorSiap){
        return true;
    }else{
        return false;
    }
}