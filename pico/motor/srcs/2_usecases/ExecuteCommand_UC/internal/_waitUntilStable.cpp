#include "../ExecuteCommand_UC.hpp"

void ExecuteCommand_UC::_waitUntilStable()
{
    while (!_vibration.isStable()) {}
}
