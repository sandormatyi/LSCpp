#ifndef MANDELBROT_STOPCHANGECOLORSCOMMAND_H
#define MANDELBROT_STOPCHANGECOLORSCOMMAND_H


#include "../command.h"

class StopChangeColorsCommand : public Command
{
public:
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
};


#endif //MANDELBROT_STOPCHANGECOLORSCOMMAND_H
