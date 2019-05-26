#ifndef MANDELBROT_STOPCHANGERESOLUTIONCOMMAND_H
#define MANDELBROT_STOPCHANGERESOLUTIONCOMMAND_H

#include "../command.h"

class StopChangeResolutionCommand : public Command
{
public:
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
};


#endif //MANDELBROT_STOPCHANGERESOLUTIONCOMMAND_H
