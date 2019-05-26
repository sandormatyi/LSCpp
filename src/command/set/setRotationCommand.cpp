#include "setRotationCommand.h"

SetRotationCommand::SetRotationCommand(coord_t amount) :
        _oldAmount(0),
        _amount(amount)
{

}

void SetRotationCommand::executeOnce(Controller &c)
{
    _oldAmount = c.getFractal().getRotAngle();
    c.getFractal().setRotAngle(_amount);
}

void SetRotationCommand::undo(Controller &c)
{
    c.getFractal().setRotAngle(_oldAmount);
}
