#include "stopProgramCommand.h"
#include "../../controller/controller.h"

void StopProgramCommand::executeOnce(Controller &c)
{
    c.setQuitFlag();
}

void StopProgramCommand::undo(Controller &c)
{

}
