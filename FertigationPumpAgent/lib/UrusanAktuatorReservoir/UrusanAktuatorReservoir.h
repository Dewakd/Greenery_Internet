#ifndef UrusanAktuatorReservoir_h
#define UrusanAktuatorReservoir_h

#include "Arduino.h"

class UrusanAktuatorReservoir {
    public:
        UrusanAktuatorReservoir(uint8_t trig);
        void mulai();
        void nyalakan();
        void padamkan();
        float bacaStatus();
    
    private:
        uint8_t _trig;
        bool _status;
};

#endif