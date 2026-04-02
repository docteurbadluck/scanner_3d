#pragma once
#include <string>

class ISender
{
public:
    virtual ~ISender() = default;
    virtual void send(const std::string &message) = 0;
};
