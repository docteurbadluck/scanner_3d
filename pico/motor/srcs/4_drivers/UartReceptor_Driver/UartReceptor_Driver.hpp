#pragma once

#include "3_interface/IReceptor.hpp"

#include <cstdint>
#include <string>

struct UartReceptor_DriverConfig
{
    uint32_t poll_timeout_us  = 100;
    char     delimiter        = '\n';
    size_t   max_msg_size     = 512;
};

class UartReceptor_Driver : public IReceptor
{
public:
    explicit UartReceptor_Driver(const UartReceptor_DriverConfig &cfg);
    ~UartReceptor_Driver() override = default;

    bool        isMessageArrived() override;
    std::string getMessage() const override;

private:
    UartReceptor_DriverConfig _cfg;
    mutable std::string       _buffer;
};
