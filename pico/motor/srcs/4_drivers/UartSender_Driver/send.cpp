#include "UartSender_Driver.hpp"

#include <cstdio>

void UartSender_Driver::send(const std::string &message)
{
    printf("%s\n", message.c_str());
}
