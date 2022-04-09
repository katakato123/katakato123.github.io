/**
 * @file BH1750FVI.cpp
 * @date 2022.01.22
 * @author LYK
 * 
 */

#include "BH1750FVI.h"

void BH1750FVI::sendCmd(uint8_t cmd)
{
    Wire.beginTransmission(this->addr);
    Wire.write(cmd);
    Wire.endTransmission();
}
void BH1750FVI::readData()
{
    if (Wire.requestFrom(addr, 2) == 2)
    {
        this->rawVal = Wire.read();
        this->rawVal <<= 8;
        this->rawVal |= Wire.read();
    }
}

BH1750FVI::BH1750FVI(uint8_t addr)
{
    this->addr = addr;
//    Wire.setClock(4000000);
//    Wire.begin();
}

void BH1750FVI::begin(uint8_t mode)
{
    this->mode = mode;
    switch (mode)
    {
    case Mode::CONT_HIGH:
    case Mode::CONT_HIGH2:
    case Mode::CONT_LOW:
        this->sendCmd(Power::ON);
        this->sendCmd(mode);
        break;
    }
}

float BH1750FVI::getLux()
{
    switch (this->mode)
    {
    case Mode::ONCE_HIGH:
    case Mode::ONCE_HIGH2:
    case Mode::ONCE_LOW:
        this->sendCmd(Power::ON);
        this->sendCmd(mode);
        break;
    }

    this->readData();
    float lux = this->rawVal / 1.2;

    switch (this->mode)
    {
    case Mode::CONT_HIGH2:
    case Mode::ONCE_HIGH2:
        lux /= 2;
        break;
    }
    return lux;
}
