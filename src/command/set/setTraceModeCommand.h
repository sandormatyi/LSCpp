#ifndef MANDELBROT_SETTRACEMODECOMMAND_H
#define MANDELBROT_SETTRACEMODECOMMAND_H


#include "../command.h"

class SetTraceModeCommand : public Command
{
public:
    SetTraceModeCommand(TraceMode traceMode);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    TraceMode _traceMode;
};


#endif //MANDELBROT_SETTRACEMODECOMMAND_H
