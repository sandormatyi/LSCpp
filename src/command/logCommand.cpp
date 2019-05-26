#include <SDL_log.h>
#include "logCommand.h"

LogCommand::LogCommand(const std::string &message) : _message(message) {}

void LogCommand::executeOnce(Controller &c)
{
    SDL_Log("%s", _message.c_str());
}

void LogCommand::undo(Controller &c)
{
}
