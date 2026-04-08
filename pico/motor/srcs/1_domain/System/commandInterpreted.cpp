#include "1_domain/System/System.hpp"

void System::commandInterpreted(bool correct)
{
    if (correct)
    {
        _current_state = EXECUTING;
    }
    else
    {
        _current_state = LISTENING;
    }
}
