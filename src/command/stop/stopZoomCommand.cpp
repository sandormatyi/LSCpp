#include "stopZoomCommand.h"
#include "../../controller/controller.h"

void StopZoomCommand::executeOnce(Controller &c)
{
    c.setZoomSpeed(1);
}

void StopZoomCommand::undo(Controller &c)
{

}
