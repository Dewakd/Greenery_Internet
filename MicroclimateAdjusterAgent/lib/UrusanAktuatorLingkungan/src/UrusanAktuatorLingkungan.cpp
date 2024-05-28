#include "UrusanAktuatorLingkungan.h"

UrusanAktuatorLingkungan::UrusanAktuatorLingkungan(uint8_t ina, uint8_t inb) : _ina(ina), _inb(inb) {}

void UrusanAktuatorLingkungan::mulai(){
    ledcSetup(0, 5000, 8); // Configure PWM channel: channel, frequency, resolution
    ledcSetup(1, 5000, 8);
    ledcAttachPin(_ina, 0);
    ledcAttachPin(_inb, 1);
}

void UrusanAktuatorLingkungan::nyalakan(uint8_t kekuatan, bool arah){ // Perintah untuk menyalakan
    _kekuatan = kekuatan; // menyimpan nilai kekuatan
    _arah = arah; // menyimpan nilai arah 
    _status = 1;
    uint8_t power = map(kekuatan, 0, 100, 80, 100); // nilai kekuatan
    Serial.printf("UrusanAktuatorLingkungan: Mengaktifkan blower dengan kekuatan: %d%% (%dpwm)\n", kekuatan, power);
    if(arah){
        ledcWrite(0, 0);
        ledcWrite(1, power);
    }else{
        ledcWrite(0, power);
        ledcWrite(1, 0);
    }
}

void UrusanAktuatorLingkungan::padamkan(){ // perintah untuk mematikan
    _status = 0;
    ledcWrite(0, 0);
    ledcWrite(1, 0);
}

// mengambil nilai arah
float UrusanAktuatorLingkungan::bacaArah(){
    return _arah;
}

//mengambil nilai kekuatan
float UrusanAktuatorLingkungan::bacaKekuatan(){
    return _kekuatan;
}

void UrusanAktuatorLingkungan::setArah(bool arah){
    _arah = arah;
}

void UrusanAktuatorLingkungan::setKekuatan(uint8_t kekuatan){
    _kekuatan = kekuatan;
}

bool UrusanAktuatorLingkungan::bacaStatus(){
    return _status;
}

void UrusanAktuatorLingkungan::anginTopan(bool arah){
    _kekuatan = 10000; // Menetapkan kekuatan maksimum mode angin topan
    _arah = arah; // menyimpan nilai arah
    _status = 1;
    Serial.printf("UrusanAktuatorLingkungan: AWAS! Mode angin topan diaktifkan dalam: "); // pesan peringatan mode angin topan akan aktif
    for(uint8_t hitungMundur = 10; hitungMundur > 0; hitungMundur--){ // looping hitung mundur
        Serial.printf("%d...", hitungMundur);
        delay(1000);
    }
    Serial.println("");
    delay(1000);
    if(arah){
        ledcWrite(0, 0);
        ledcWrite(1, 255);
    }else{
        ledcWrite(0, 255);
        ledcWrite(1, 0);
    }
}