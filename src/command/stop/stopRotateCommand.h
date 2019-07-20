#ifndef MANDELBROT_STOPROTATECOMMAND_H
#define MANDELBROT_STOPROTATECOMMAND_H


#include "../command.h"

class StopRotateCommand : public Command
{
public:
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
};


#endif //MANDELBROT_STOPROTATECOMMAND_H
