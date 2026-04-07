#include "CommandReceptor_UC.hpp"

CommandReceptor_UC::CommandReceptor_UC(IComputer &uploader, SendToComputer_UC &sender)
    : _computer(uploader), _sender(sender)
{}
