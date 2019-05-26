#include "setTraceModeCommand.h"

SetTraceModeCommand::SetTraceModeCommand(TraceMode traceMode) : _traceMode(traceMode) {}

void SetTraceModeCommand::executeOnce(Controller &c)
{
    c.getFractalRenderer().setTraceMode(_traceMode);
}

void SetTraceModeCommand::undo(Controller &c)
{
    // TODO
}
