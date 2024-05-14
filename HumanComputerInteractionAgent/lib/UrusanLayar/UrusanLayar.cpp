#include "UrusanLayar.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

UrusanLayar::UrusanLayar() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void UrusanLayar::mulai() {
    Wire.begin();
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("UrusanLayar: Layar gagal digunakan! Cek pengkabelan."));
        while(1);
    }
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.display();
}

void UrusanLayar::updateTemperatureAndHumidity(float temperature, float humidity) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Suhu:");
    display.print(temperature, 1); // One decimal place for temperature
    display.println("°C");
    display.print("Kelembapan:");
    display.print(humidity, 1); // One decimal place for humidity
    display.println("%");
    display.display();
}

void UrusanLayar::updateFanStatus(int speed, bool isUpDirection) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Kec. Kipas: ");
    display.print(speed);
    display.println("%");
    display.print("Arah: ");
    display.println(isUpDirection ? "Up" : "Down");
    display.display();
}

void UrusanLayar::updateWaterReservoir(float level) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Level Air: ");
    display.print(level);
    display.println("%");
    display.display();
}

void UrusanLayar::updatePumpStatus(bool isOn) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Status Pompa: ");
    display.println(isOn ? "ON" : "OFF");
    display.display();
}