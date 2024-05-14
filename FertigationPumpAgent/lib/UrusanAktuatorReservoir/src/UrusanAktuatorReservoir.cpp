#include "UrusanAktuatorReservoir.h"

UrusanAktuatorReservoir::UrusanAktuatorReservoir(uint8_t trig) : _trig(trig) {}

void UrusanAktuatorReservoir::mulai(){
    pinMode(_trig, OUTPUT);
}

void UrusanAktuatorReservoir::nyalakan(){
    _status = 1;
    digitalWrite(_trig, HIGH);
}

void UrusanAktuatorReservoir::padamkan(){
    _status = 0;
    digitalWrite(_trig, LOW);
}

float UrusanAktuatorReservoir::bacaStatus(){
    return _status;
}