#include "changeRotationCommand.h"

ChangeRotationCommand::ChangeRotationCommand(coord_t amount) : _amount(amount) {}

void ChangeRotationCommand::executeOnce(Controller &c)
{
    c.getFractal().rotate(_amount);
}

void ChangeRotationCommand::undo(Controller &c)
{
    c.getFractal().rotate(-_amount);
}