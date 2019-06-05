#include "logCommand.h"
#include <iostream>

LogCommand::LogCommand(const std::string &message) : _message(message) {}

void LogCommand::executeOnce(Controller &c)
{
    std::cout << _message << std::endl;
}

void LogCommand::undo(Controller &c)
{
}
