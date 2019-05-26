#include "changeColorsCommand.h"
#include "../../colors.h"

ChangeColorsCommand::ChangeColorsCommand(float_color_t deltaColor) :
        _deltaColor(deltaColor)
{
}

void ChangeColorsCommand::executeOnce(Controller &c)
{
    changeColors(_deltaColor.r, _deltaColor.g, _deltaColor.b, _deltaColor.a);
}

void ChangeColorsCommand::undo(Controller &c)
{
    changeColors(-_deltaColor.r, -_deltaColor.g, -_deltaColor.b, -_deltaColor.a);
}
