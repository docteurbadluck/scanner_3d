#pragma once

#include "3_interface/IReceptor.hpp"

#include <cstdint>
#include <string>

struct UsbReceptor_DriverConfig
{
    uint32_t poll_timeout_us  = 100;
    char     delimiter        = '\n';
    size_t   max_msg_size     = 512;
};

class UsbReceptor_Driver : public IReceptor
{
public:
    explicit UsbReceptor_Driver(const UsbReceptor_DriverConfig &cfg);
    ~UsbReceptor_Driver() override = default;

    bool        isMessageArrived() override;
    std::string getMessage() const override;

private:
    UsbReceptor_DriverConfig _cfg;
    mutable std::string      _buffer;
};
