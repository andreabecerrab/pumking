#ifndef RINGLED_H
#define RINGLED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


class RingLed {
private:
    Adafruit_NeoPixel ring;
    int ledsCount;

public:
    RingLed(int ringPin, int ledsCount);
    void begin();
    void changeLedColor(int n, int r, int g, int b);
    void changeRingColor(int r, int g, int b, int d);
    void turnRingOff();
    // measure interactions
    void flashing();
    void madurate();
    // phone interactions
    void greenDance();
    void blueDance();
    void roseDancing();
    // extra
    void orangePumpking(int d);
    void rainbow();
};

#endif
