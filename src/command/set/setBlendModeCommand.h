#ifndef MANDELBROT_SETBLENDMODECOMMAND_H
#define MANDELBROT_SETBLENDMODECOMMAND_H


#include "../command.h"

class SetBlendModeCommand : public Command
{
public:
    SetBlendModeCommand(BlendMode blendMode);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    BlendMode _blendMode;
};


#endif //MANDELBROT_SETBLENDMODECOMMAND_H
