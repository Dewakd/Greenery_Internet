#ifndef UrusanSensorLingkungan_h
#define UrusanSensorLingkungan_h

#include <Wire.h>
#include <Adafruit_SHT31.h>

class UrusanSensorLingkungan {
    public:
        UrusanSensorLingkungan();
        bool mulai();
        float bacaSuhu();
        float bacaKelembapan();
        bool apakahSensorSiap();
    
    private:
        Adafruit_SHT31 sht31;
        bool sensorSiap = false;
};

#endif