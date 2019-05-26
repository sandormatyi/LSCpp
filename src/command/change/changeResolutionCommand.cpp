#include "changeResolutionCommand.h"

ChangeResolutionCommand::ChangeResolutionCommand(coord_t amount) :
        _amount(amount),
        _actualAmount(amount)
{
}

void ChangeResolutionCommand::executeOnce(Controller &c)
{
    _actualAmount = c.getFractal().changeMaxN(_amount);
}

void ChangeResolutionCommand::undo(Controller &c)
{
    c.getFractal().changeMaxN(-_actualAmount);
}