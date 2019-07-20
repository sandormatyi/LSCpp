#ifndef MANDELBROT_STOPPROGRAMCOMMAND_H
#define MANDELBROT_STOPPROGRAMCOMMAND_H


#include "../command.h"

class StopProgramCommand : public Command
{
public:
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
};

#endif //MANDELBROT_STOPPROGRAMCOMMAND_H
