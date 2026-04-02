#pragma once
#include <string>

class IReceptor
{
public:
    virtual ~IReceptor() = default;
    virtual bool        isMessageArrived() = 0;
    virtual std::string getMessage() const = 0;
};
