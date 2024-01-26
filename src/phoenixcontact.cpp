#include "../include/phoenixcontact.h"
#include <iostream>

std::bitset<16>::reference PhoenixData::operator[](const size_t index)
{
    // Because the two bytes are inverted we need to customize the operator of the Phoenix Bits
    return bits[index >= 8 ? index%8 : index+8];
}

PhoenixDevice::PhoenixDevice(uint16_t reg, size_t byteCount, const std::string& ipAddress)
    : _register(reg)
    , _data(byteCount)
    , _context(nullptr)
    , _isConnected(false)
{
    _context = modbus_new_tcp(ipAddress.c_str(), 502);
    if (modbus_connect(_context) == -1)
        modbus_free(_context);
    else
        _isConnected = true;
}

bool PhoenixDevice::isConnected() { return _isConnected; }

std::bitset<16>::reference PhoenixDevice::operator[](const size_t index)
{
    const auto indexVector = static_cast<size_t>(static_cast<float>(index)/16.0f);
    return _data[indexVector][index%16];
}

void PhoenixDevice::writeData()
{
    for(size_t i(0); i<_data.size(); i++)
    {
        auto status = modbus_write_register(_context, _register+static_cast<uint16_t>(i), _data[i].getValue());
        if(status == -1)
            std::cout << "Could not write value from address '" << std::to_string(_register+i) << "': " << modbus_strerror(errno) << std::endl;
    }
}

void PhoenixDevice::disconnect()
{
    if (_isConnected)
    {
        modbus_close(_context);
        modbus_free(_context);
    }
}