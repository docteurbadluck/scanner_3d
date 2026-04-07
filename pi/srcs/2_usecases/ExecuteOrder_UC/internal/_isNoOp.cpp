#include "../ExecuteOrder_UC.hpp"

bool ExecuteOrder_UC::_isNoOp(const std::string &cmd) const
{
    return cmd == "PING" || cmd == "GET_STATUS" || cmd == "PONG";
}
