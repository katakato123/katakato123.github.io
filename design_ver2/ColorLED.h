#ifndef _ColorLED_H
#define _ColorLED_H

#include <Arduino.h>

class ColorLED
{
private:
    void actualSetColor();

protected:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;
    uint8_t r = 0xFF;
    uint8_t g = 0xFF;
    uint8_t b = 0xFF;
    bool state = false;

public:
    ColorLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t color);
    uint32_t getColor();
    void setState(bool state);
    bool getState();
};

#endif
