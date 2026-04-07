#pragma once

#include "3_interface/ISender.hpp"

#include <string>

class UsbSender_Driver : public ISender
{
public:
    UsbSender_Driver() = default;
    ~UsbSender_Driver() override = default;

    void send(const std::string &message) override;
};
