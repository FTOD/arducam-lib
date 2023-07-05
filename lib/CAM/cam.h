#include <Arduino.h>
#include "config_format.h"
#include <SPI.h>
class Cam
{
public:
    // only some configuration, no real setup
    Cam(int cs) : CS(cs){};

    // Setup all necessary buses
    void setup();
    void configure_and_init();
    void apply_config(const struct reg_config config[]);
    inline void flush_fifo() { SPI_reg_wr(0x04, 0x01); };
    inline void start_capture() { SPI_reg_wr(0x04, 0x02); };
    inline uint8_t is_capture_done() { return SPI_reg_rd(0x41) & 0x08; };
    inline void set_fifo_burst() { SPI.transfer(0x3C); };
    uint32_t read_fifo_length();
    uint8_t read_fifo_burst(size_t size, uint8_t *buf);

    // Useful constants to mark return value
    // const int OK = 0;
    // const int I2C_ERR = -1;

    // Camera related constants
    const byte SENSOR_ADDR = 0x60;
    const byte OV2640_CHIPID_HIGH = 0x0A;
    const byte OV2640_CHIPID_LOW = 0x0B;

private:
    // I2C read/write functions
    uint8_t i2c8Wr8(uint8_t addr, uint8_t data);
    uint8_t i2c8Wr16(uint8_t addr, uint16_t data);
    uint8_t i2c8Rd8(uint8_t addr, uint8_t *data);
    uint8_t i2c8Rd16(uint8_t addr, uint16_t *data);
    uint8_t i2c16Wr8(uint16_t addr, uint8_t data);
    uint8_t i2c16Wr16(uint16_t addr, uint16_t data);
    uint8_t i2c16Rd8(uint16_t addr, uint8_t *data);
    uint8_t i2c16Rd16(uint16_t addr, uint16_t *data);

    // SPI read/write functions
    uint8_t SPI_wr(int addr, int data);
    uint8_t SPI_rd(int addr);
    uint8_t SPI_reg_wr(int addr, int data);
    uint8_t SPI_reg_rd(int addr);

private:
    int CS;
};