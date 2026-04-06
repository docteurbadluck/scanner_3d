#include "UsbSender_Driver.hpp"

#include <cstdio>

void UsbSender_Driver::send(const std::string &message)
{
    printf("%s\n", message.c_str());
}
