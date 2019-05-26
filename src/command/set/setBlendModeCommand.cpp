#include "setBlendModeCommand.h"

SetBlendModeCommand::SetBlendModeCommand(BlendMode blendMode) :
_blendMode(blendMode)
{
}

void SetBlendModeCommand::executeOnce(Controller &c)
{
    c.getFractalRenderer().setBlendMode(_blendMode);
}

void SetBlendModeCommand::undo(Controller &c)
{
    // TODO
}
