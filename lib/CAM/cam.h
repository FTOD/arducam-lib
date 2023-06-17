#include <Arduino.h>
class Cam
{
public:
    Cam();

    // Useful constants to mark return value
    // const int OK = 0;
    // const int I2C_ERR = -1;

    // Camera related constants
    const byte SENSOR_ADDR = 0x60;

private:
    // Camera related controls are on the I2C bus
    uint8_t i2cWr8(uint16_t addr, uint8_t data);
    uint8_t i2cWr16(uint16_t addr, uint16_t data);
    uint8_t i2cRd8(uint16_t addr, uint8_t *data);
    uint8_t i2cRd16(uint16_t addr, uint16_t *data);
};