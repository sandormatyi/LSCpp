#include "stopChangeColorsCommand.h"
#include "../../controller/controller.h"

void StopChangeColorsCommand::executeOnce(Controller &c)
{
    c.setDeltaColor({0,0,0,0});
}

void StopChangeColorsCommand::undo(Controller &c)
{

}
