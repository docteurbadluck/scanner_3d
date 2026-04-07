#include "1_domain/System/System.hpp"

bool System::resultCommand(int res_code)
{
    _current_state = LISTENING;
    return (res_code == 0);
}
