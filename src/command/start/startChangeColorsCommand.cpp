#include "startChangeColorsCommand.h"

StartChangeColorsCommand::StartChangeColorsCommand(float_color_t deltaColor) :
        _deltaColor(deltaColor)
{
}

void StartChangeColorsCommand::executeOnce(Controller &c)
{
    c.setDeltaColor(_deltaColor);
}

void StartChangeColorsCommand::undo(Controller &c)
{
    // TODO
    c.setDeltaColor(-_deltaColor);
}
