#include "setEnableRenderCommand.h"

SetEnableRenderCommand::SetEnableRenderCommand(bool enable)
{
    _enable = enable;
}

void SetEnableRenderCommand::executeOnce(Controller &c)
{
    c.getFractalRenderer().setEnableRender(_enable);
}

void SetEnableRenderCommand::undo(Controller &c)
{
    // TODO
}
