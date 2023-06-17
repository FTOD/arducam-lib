#ifndef CONFIG_FORMAT_H
#define CONFIG_FORMAT_H

#include <Arduino.h>
// End the config list by writting 0xff to 0xff
struct reg_config
{
    uint8_t reg;
    uint8_t val;
};

#endif // CONFIG_FORMAT_H
