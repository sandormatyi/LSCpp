#include "changeResolutionCommand.h"
#include "../../controller/controller.h"
#include "../../fractal/fractal.h"

ChangeResolutionCommand::ChangeResolutionCommand(fractal_value_t amount) :
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