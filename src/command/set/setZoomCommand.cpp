#include "setZoomCommand.h"

SetZoomCommand::SetZoomCommand(coord_t amount) :
        _amount(amount),
        _oldAmount(-1)
{
}

void SetZoomCommand::executeOnce(Controller &c)
{
    _oldAmount = c.getFractal().getZoom();
    c.getFractal().setZoom(_amount);
}

void SetZoomCommand::undo(Controller &c)
{
    if (_oldAmount > 0)
        c.getFractal().setZoom(_oldAmount);
}
