#pragma once

#include "3_interface/ISender.hpp"

#include <string>

class UartSender_Driver : public ISender
{
public:
    UartSender_Driver();
    ~UartSender_Driver() override = default;

    void send(const std::string &message) override;
};
