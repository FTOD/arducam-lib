#include "cam.h"
#include <Wire.h>
#include <SPI.h>
#include "OV2640_QVGA.h"

void Cam::setup()
{
    // setup SPI bus
    pinMode(CS, OUTPUT);
    SPI.begin();
    digitalWrite(CS, LOW);
    delay(100);
    while (1)
    {
        // test the SPI bus with 0x00 the test register
        SPI_reg_wr(0x00, 0x55);
        int res = SPI_reg_rd(0x00);
        if (res != 0x55)
        {
            Serial.println("SPI bus test failed, retrying...");
            delay(3000);
        }
        else
        {
            Serial.println("SPI bus OK");
            break;
        }
    }
    digitalWrite(CS, HIGH);

    // setup I2C bus
    Wire.begin();
    while (1)
    {
        i2c8Wr8(0xff, 0x01);
        uint8_t vid;
        i2c8Rd8(OV2640_CHIPID_HIGH, &vid);
        uint8_t pid;
        i2c8Rd8(OV2640_CHIPID_LOW, &pid);
        if ((vid != 0x26) && ((pid != 0x41) || (pid != 0x42)))
        {
            Serial.println(F("OV2640 module not found on I2C, retrying..."));
            delay(3000);
        }
        else
        {
            Serial.println(F("OV2640 module checked with I2C chip iD. I2C test OK."));
            break;
        }
    }

    Serial.println(F("SPI and I2C setup finished, starting configuration..."));
}

void Cam::configure_and_init()
{
    // Switch to bank 1
    i2c8Wr8(0xff, 0x01);
    // Reset all to the factory default
    i2c8Wr8(0x12, 0x80);

    // JPEG mode
    // wrSensorRegs8_8(OV2640_JPEG_INIT);
    // wrSensorRegs8_8(OV2640_YUV422);
    // wrSensorRegs8_8(OV2640_JPEG);
    // wrSensorReg8_8(0xff, 0x01);
    // wrSensorReg8_8(0x15, 0x00);
    // wrSensorRegs8_8(OV2640_320x240_JPEG);

    // Bit array mode
    apply_config(OV2640_QVGA);

    // TODO i don't know why but copied from Arduino code
    i2c16Wr8(0x3818, 0x81);
    i2c16Wr8(0x3621, 0xA7);
}

void Cam::apply_config(const struct reg_config config[])
{
    for (int i = 0; config[i].reg != 0xff || config[i].val != 0xff; i++)
    {
        i2c8Wr8(config[i].reg, config[i].val);
        delay(1);
    }
}

uint8_t Cam::i2c8Wr8(uint8_t addr, uint8_t data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr & 0xFF);
    // write the data
    Wire.write(data & 0xFF);
    // delay(1);
    return Wire.endTransmission();
}

uint8_t Cam::i2c8Wr16(uint8_t addr, uint16_t data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr & 0xFF);
    // write the data
    Wire.write(data >> 8);
    Wire.write(data & 0xFF);
    // delay(1);
    return Wire.endTransmission();
}

uint8_t Cam::i2c8Rd8(uint8_t addr, uint8_t *data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom(SENSOR_ADDR >> 1, 1);
    *data = Wire.read();
    return 0;
}

uint8_t Cam::i2c8Rd16(uint8_t addr, uint16_t *data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom(SENSOR_ADDR >> 1, 2);
    *data = Wire.read() << 8;
    *data |= Wire.read();
    return 0;
}

uint8_t Cam::i2c16Wr8(uint16_t addr, uint8_t data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr >> 8);
    Wire.write(addr & 0xFF);
    // write the data
    Wire.write(data & 0xFF);
    // delay(1);
    return Wire.endTransmission();
}

uint8_t Cam::i2c16Wr16(uint16_t addr, uint16_t data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr >> 8);
    Wire.write(addr & 0xFF);
    // write the data
    Wire.write(data >> 8);
    Wire.write(data & 0xFF);
    // delay(1);
    return Wire.endTransmission();
}

uint8_t Cam::i2c16Rd8(uint16_t addr, uint8_t *data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr >> 8);
    Wire.write(addr & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom(SENSOR_ADDR >> 1, 1);
    *data = Wire.read();
    return 0;
}

uint8_t Cam::i2c16Rd16(uint16_t addr, uint16_t *data)
{
    Wire.beginTransmission(SENSOR_ADDR >> 1);
    Wire.write(addr >> 8);
    Wire.write(addr & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom(SENSOR_ADDR >> 1, 2);
    *data = Wire.read() << 8;
    *data |= Wire.read();
    return 0;
}

uint8_t Cam::SPI_wr(int addr, int data)
{
    digitalWrite(CS, LOW);
    SPI.transfer(addr);
    SPI.transfer(data);
    digitalWrite(CS, HIGH);
    return 0;
}

uint8_t Cam::SPI_rd(int addr)
{
    digitalWrite(CS, LOW);
    SPI.transfer(addr);
    uint8_t res;
    res = SPI.transfer(0x00);
    digitalWrite(CS, HIGH);
    return res;
}

uint8_t Cam::SPI_reg_wr(int addr, int data)
{
    SPI_wr(addr | 0x80, data);
    return 0;
}

uint8_t Cam::SPI_reg_rd(int addr)
{
    return SPI_rd(addr & 0x7F);
}

uint32_t Cam::read_fifo_length()
{
    uint32_t len1, len2, len3, length = 0;
    len1 = SPI_rd(0x42);
    len2 = SPI_rd(0x43);
    len3 = SPI_rd(0x44) & 0x7f;
    length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
    return length;
}
uint8_t Cam::read_fifo_burst(size_t size, uint8_t *buf)
{
    uint32_t len = read_fifo_length();
    digitalWrite(CS, LOW);
    set_fifo_burst();
    uint8_t temp0, temp1;
    int i = 0;
    while (len-- && i < size)
    {
        temp1 = SPI.transfer(0x00);
        delayMicroseconds(12);
        temp0 = SPI.transfer(0x00);
        buf[i++] = temp0;
        buf[i++] = temp1;
        len--;
    }
    digitalWrite(CS, HIGH);
    return 0;
}

uint8_t Cam::read_fifo_burst_grayscale(size_t size, uint8_t *buf)
{
    uint32_t len = read_fifo_length();
    digitalWrite(CS, LOW);
    set_fifo_burst();
    uint16_t temp0, temp1;
    uint16_t pixel;
    int i = 0;
    while (len > 0 && i < size)
    {
        temp1 = (uint16_t)SPI.transfer(0x00);
        delayMicroseconds(12);
        temp0 = (uint16_t)SPI.transfer(0x00);
        pixel = (temp1 << 8) | temp0;
        uint16_t red = ((pixel & 0xF800) >> 11) << 3;
        uint16_t green = ((pixel & 0x07E0) >> 5) << 2;
        uint16_t blue = ((pixel & 0x001F)) << 3;

        double grayscale_f = ((0.298 * (double)red) + (0.5870 * (double)green) + (0.11 * (double)blue));
        uint8_t grayscale = (uint8_t)grayscale_f;
        buf[i++] = grayscale;
        len -= 2;
    }
    digitalWrite(CS, HIGH);
    return 0;
}