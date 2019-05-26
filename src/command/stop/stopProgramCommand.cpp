#include "stopProgramCommand.h"

void StopProgramCommand::executeOnce(Controller &c)
{
    c.setQuitFlag();
}

void StopProgramCommand::undo(Controller &c)
{

}
