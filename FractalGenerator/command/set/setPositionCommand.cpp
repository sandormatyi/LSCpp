#include "setPositionCommand.h"
#include "../../fractal/fractal.h"
#include "../../controller/controller.h"

SetPositionCommand::SetPositionCommand(coord_t x, coord_t y) :
    _position{ x, y }
{
}

void SetPositionCommand::executeOnce(Controller &c)
{
    Fractal &f = c.getFractal();

    _oldPosition = { f.getXCenter(), f.getYCenter() };
    f.setXCenter(_position.x);
    f.setYCenter(_position.y);
}

void SetPositionCommand::undo(Controller &c)
{
    Fractal &f = c.getFractal();

    f.setXCenter(_oldPosition.x);
    f.setYCenter(_oldPosition.y);
}
