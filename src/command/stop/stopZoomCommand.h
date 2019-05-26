#ifndef MANDELBROT_STOPZOOMCOMMAND_H
#define MANDELBROT_STOPZOOMCOMMAND_H


#include "../command.h"

class StopZoomCommand : public Command
{
public:
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
};


#endif //MANDELBROT_STOPZOOMCOMMAND_H
