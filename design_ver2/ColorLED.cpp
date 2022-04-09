#include "ColorLED.h"


ColorLED::ColorLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    pinMode(this->redPin, OUTPUT);
    pinMode(this->greenPin, OUTPUT);
    pinMode(this->bluePin, OUTPUT);
}

void ColorLED::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->actualSetColor();
}

void ColorLED::setColor(uint32_t color)
{
    this->r = color >> 16;
    this->g = color >> 8;
    this->b = color;
    this->actualSetColor();
}

uint32_t ColorLED::getColor()
{
    uint32_t color = 0;
    color |= this->r << 16;
    color |= this->g << 8;
    color |= this->b;
    return color;
}

void ColorLED::setState(bool state)
{
    this->state = state;
    this->actualSetColor();
}
bool ColorLED::getState()
{
    return state;
}

void ColorLED::actualSetColor()
{
    if (this->state)
    {
        analogWrite(this->redPin, this->r);
        analogWrite(this->greenPin, this->g);
        analogWrite(this->bluePin, this->b);
    }
    else
    {
        analogWrite(this->redPin, 0);
        analogWrite(this->greenPin, 0);
        analogWrite(this->bluePin, 0);
    }
}
