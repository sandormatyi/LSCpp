#include "startRotateCommand.h"
#include "../../controller/controller.h"

StartRotateCommand::StartRotateCommand(coord_t amount) : _amount(amount) {}

void StartRotateCommand::executeOnce(Controller &c)
{
    c.setDeltaRot(_amount);
}

void StartRotateCommand::undo(Controller &c)
{
    c.setDeltaRot(0);
}
