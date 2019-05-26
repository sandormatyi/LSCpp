#include "startChangeResolutionCommand.h"

StartChangeResolutionCommand::StartChangeResolutionCommand(coord_t amount) :
        _amount(amount)
{
}

void StartChangeResolutionCommand::executeOnce(Controller &c)
{
    c.setDeltaN(_amount);
}

void StartChangeResolutionCommand::undo(Controller &c)
{
    c.setDeltaN(0);
}
