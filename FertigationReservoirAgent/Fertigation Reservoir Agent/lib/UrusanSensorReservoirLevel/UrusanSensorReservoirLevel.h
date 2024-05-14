#ifndef UrusanSensorReservoirLevel_h
#define UrusanSensorReservoirLevel_h

#include <Arduino.h>
#include <HCSR04.h>

class UrusanSensorReservoirLevel {
    public:
        UrusanSensorReservoirLevel(float tinggiReservoir, const int trigPin, const int echoPin);
        bool mulai();
        float bacaLevel(float jarak);
        float bacaJarak();
        bool apakahSensorSiap();
    
    private:
        float _tinggiReservoir;
        bool _sensorSiap = false;
        const int _trigPin;
        const int _echoPin;
        HCSR04 _hcsr04;
};

#endif