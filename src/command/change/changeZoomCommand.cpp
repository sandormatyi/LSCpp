#include "changeZoomCommand.h"
#include "../../controller/controller.h"
#include "../../fractal/fractal.h"

ChangeZoomCommand::ChangeZoomCommand(coord_t amount) :
        _amount(amount)
{
}

void ChangeZoomCommand::executeOnce(Controller &c)
{
    c.getFractal().zoom(_amount);
}

void ChangeZoomCommand::undo(Controller &c)
{
    c.getFractal().zoom(1 / _amount);
}