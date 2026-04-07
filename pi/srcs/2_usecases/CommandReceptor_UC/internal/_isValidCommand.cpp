#include "../CommandReceptor_UC.hpp"

bool CommandReceptor_UC::_isValidCommand(const Commands &cmds) const
{
    for (size_t i = 0; i < cmds.valid_command.size(); i++)
    {
        if (_message == cmds.valid_command[i])
            return true;
    }
    return false;
}
