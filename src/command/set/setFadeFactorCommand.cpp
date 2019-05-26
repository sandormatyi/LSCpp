#include "setFadeFactorCommand.h"

SetFadeFactorCommand::SetFadeFactorCommand(coord_t amount)
{
    _amount = amount;
}

void SetFadeFactorCommand::executeOnce(Controller &c)
{
    c.getFractalRenderer().setFadeFactor(_amount);
}

void SetFadeFactorCommand::undo(Controller &c)
{
    // TODO
}
