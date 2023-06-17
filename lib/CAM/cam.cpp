#include "cam.h"
#include <Wire.h>
uint8_t Cam::i2cWr8(uint16_t addr, uint8_t data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    // if the address is greater than 0xFF, we need to send the address in two bytes
    if (addr > 0xFF)
    {
        Wire.write(addr >> 8);
        Wire.write(addr & 0xFF);
    }
    else
    {
        Wire.write(addr);
    }
    // write the data
    Wire.write(data & 0xFF);
    // delay(1);
    return Wire.endTransmission();
}

uint8_t Cam::i2cWr16(uint16_t addr, uint16_t data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    // if the address is greater than 0xFF, we need to send the address in two bytes
    if (addr > 0xFF)
    {
        Wire.write(addr >> 8);
        Wire.write(addr & 0xFF);
    }
    else
    {
        Wire.write(addr);
    }
    // write the data
    Wire.write(data >> 8);
    Wire.write(data & 0xFF);
    // delay(1);
    return Wire.endTransmission();
}