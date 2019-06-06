#include "stopChangeResolutionCommand.h"
#include "../../controller/controller.h"

void StopChangeResolutionCommand::executeOnce(Controller &c)
{
    c.setDeltaN(0);
}

void StopChangeResolutionCommand::undo(Controller &c)
{
    // TODO ?
}
