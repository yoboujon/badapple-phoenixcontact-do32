#ifndef HEADER_PHOENIX_CONTACT
#define HEADER_PHOENIX_CONTACT

#include <bitset>
#include <vector>
#include <string>
#include <modbus.h>

union PhoenixData
{
public:
    PhoenixData()
        : value(0)
    {
    }
    std::bitset<16>::reference operator[](const size_t index);
    uint16_t getValue() noexcept { return value; };
    void setValue(uint16_t v) noexcept { value = v; };
    uint8_t getByte(const size_t index) { return bytes[index]; };
    void setByte(const size_t index, uint8_t v) { bytes[index] = v; };

private:
    uint16_t value;
    uint8_t bytes[2];
    std::bitset<16> bits;
};

class PhoenixDevice
{
public:
    PhoenixDevice(uint16_t reg, size_t byteCount, const std::string &ipAddress);
    bool isConnected();
    std::bitset<16>::reference operator[](size_t index);
    void writeData();
    void disconnect();

private:
    uint16_t _register;
    std::vector<PhoenixData> _data;
    modbus_t *_context;
    bool _isConnected;
};

#endif // HEADER_PHOENIX_CONTACT