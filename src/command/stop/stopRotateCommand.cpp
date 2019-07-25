#include "stopRotateCommand.h"
#include "../../controller/controller.h"

void StopRotateCommand::executeOnce(Controller &c)
{
    c.setDeltaRot(0);
}

void StopRotateCommand::undo(Controller &c)
{

}
