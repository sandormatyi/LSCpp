#include "setResolutionCommand.h"
#include "../../controller/controller.h"
#include "../../fractal/fractal.h"

SetResolutionCommand::SetResolutionCommand(coord_t amount) :
    _amount(amount),
    _oldAmount(-1)
{
}

void SetResolutionCommand::executeOnce(Controller &c)
{
    _oldAmount = c.getFractal().getMaxN();
    c.getFractal().setMaxN(_amount);
}

void SetResolutionCommand::undo(Controller &c)
{
    if (_oldAmount > 0) {
        c.getFractal().setMaxN(_oldAmount);
    }
}
