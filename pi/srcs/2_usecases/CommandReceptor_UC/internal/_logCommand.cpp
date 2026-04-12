#include "../CommandReceptor_UC.hpp"
#include <cstdio>
#include <ctime>

void CommandReceptor_UC::_logCommand(bool valid) const
{
    char      buf[32];
    time_t    now = time(nullptr);
    struct tm *tm = localtime(&now);
    strftime(buf, sizeof(buf), "%H:%M:%S", tm);
    if (valid)
        printf("[%s] command: %s\n", buf, _message.c_str());
    else
        printf("[%s] command: %s (invalid)\n", buf, _message.c_str());
}
