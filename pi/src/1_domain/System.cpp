#include "System.hpp"

System::System() : _current_state(INITIALIZATION)
{
}

state System::get_state() const
{
	return _current_state;
}

void System::ready()
{
	_current_state = READY;
}

void System::processing()
{
	_current_state = PROCESSING;
}

void System::sending()
{
	_current_state = SENDING;
}

void System::reset()
{
	_current_state = INITIALIZATION;
}
