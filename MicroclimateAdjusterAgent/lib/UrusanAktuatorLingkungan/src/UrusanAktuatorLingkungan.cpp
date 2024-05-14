#include "UrusanAktuatorLingkungan.h"

UrusanAktuatorLingkungan::UrusanAktuatorLingkungan(uint8_t ina, uint8_t inb) : _ina(ina), _inb(inb) {}

void UrusanAktuatorLingkungan::mulai(){
    ledcSetup(0, 5000, 8); // Configure PWM channel: channel, frequency, resolution
    ledcSetup(1, 5000, 8);
    ledcAttachPin(_ina, 0);
    ledcAttachPin(_inb, 1);
}

void UrusanAktuatorLingkungan::nyalakan(uint8_t kekuatan, bool arah){
    _kekuatan = kekuatan;
    _arah = arah;
    uint8_t power = map(kekuatan, 0, 100, 80, 100);
    Serial.printf("UrusanAktuatorLingkungan: Mengaktifkan blower dengan kekuatan: %d%% (%dpwm)\n", kekuatan, power);
    if(arah){
        ledcWrite(0, 0);
        ledcWrite(1, power);
    }else{
        ledcWrite(0, power);
        ledcWrite(1, 0);
    }
}

void UrusanAktuatorLingkungan::padamkan(){
    ledcWrite(0, 0);
    ledcWrite(1, 0);
}

float UrusanAktuatorLingkungan::bacaArah(){
    return _arah;
}

float UrusanAktuatorLingkungan::bacaKekuatan(){
    return _kekuatan;
}

void UrusanAktuatorLingkungan::anginTopan(bool arah){
    _kekuatan = 10000;
    _arah = arah;
    Serial.printf("UrusanAktuatorLingkungan: AWAS! Mode angin topan diaktifkan dalam: ");
    for(uint8_t hitungMundur = 10; hitungMundur > 0; hitungMundur--){
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