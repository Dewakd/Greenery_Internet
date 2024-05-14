#ifndef UrusanAktuatorLingkungan_h
#define UrusanAktuatorLingkungan_h

#include "Arduino.h"

class UrusanAktuatorLingkungan {
    public:
        UrusanAktuatorLingkungan(uint8_t ina, uint8_t inb);
        void mulai();
        void nyalakan(uint8_t kekuatan, bool arah);
        void padamkan();
        float bacaKekuatan();
        float bacaArah();
        void anginTopan(bool arah);
    
    private:
        uint8_t _ina;
        uint8_t _inb;
        float _kekuatan;
        float _arah;
};

#endif