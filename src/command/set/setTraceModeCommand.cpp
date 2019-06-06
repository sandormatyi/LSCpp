#include "setTraceModeCommand.h"
#include "../../controller/controller.h"
#include "../../render/fractalRenderer.h"

SetTraceModeCommand::SetTraceModeCommand(TraceMode traceMode) :
    _traceMode(traceMode)
{
}

void SetTraceModeCommand::executeOnce(Controller &c)
{
    c.getFractalRenderer().setTraceMode(_traceMode);
}

void SetTraceModeCommand::undo(Controller &c)
{
    // TODO
}
