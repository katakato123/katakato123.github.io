/**
 * @file BH1750FVI.h
 * @date 2022.01.22
 * @author LYK
 * 
 */

#ifndef _BH1750FVI_H
#define _BH1750FVI_H

#include <Wire.h>

class BH1750FVI
{
protected:
    enum Power
    {
        DOWN = 0b00000000,
        ON = 0b00000001,
        RESET = 0b00000111
    };
    uint8_t addr;
    uint8_t mode;
    uint16_t rawVal;
    void sendCmd(uint8_t cmd);
    void readData();

public:
    enum Addr
    {
        L = 0b00100011,
        H = 0b01011100
    };
    enum Mode
    {
        CONT_HIGH = 0b00010000,
        CONT_HIGH2 = 0b00010001,
        CONT_LOW = 0b00010011,
        ONCE_HIGH = 0b00100000,
        ONCE_HIGH2 = 0b00100001,
        ONCE_LOW = 0b00100011
    };
    BH1750FVI(uint8_t addr = Addr::L);
    void begin(uint8_t mode = Mode::CONT_HIGH);
    float getLux();
};

#endif
