#include "startZoomCommand.h"
#include "../../controller/controller.h"

StartZoomCommand::StartZoomCommand(coord_t amount) : _amount(amount) {}

void StartZoomCommand::executeOnce(Controller &c)
{
    c.setZoomSpeed(_amount);
}

void StartZoomCommand::undo(Controller &c)
{
    c.setZoomSpeed(0);
}
