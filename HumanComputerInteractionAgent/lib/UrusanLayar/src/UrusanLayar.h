#ifndef UrusanLayar_h
#define UrusanLayar_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class UrusanLayar {
    public:
        UrusanLayar();
        void mulai();
        void updateTemperatureAndHumidity(float temperature, float humidity);
        void updateFanStatus(int speed, bool isUpDirection);
        void updateWaterReservoir(float level);
        void updatePumpStatus(bool isOn);
    
    private:
        Adafruit_SSD1306 display;
};

#endif