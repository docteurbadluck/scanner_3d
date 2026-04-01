#pragma once
#include <string>

class IReceptor
{
private:
public:
    virtual ~IReceptor() = default;
    virtual bool is_message_arrived() = 0;
    virtual std::string get_message() const = 0;
};
