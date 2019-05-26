#ifndef MANDELBROT_CHANGEZOOMCOMMAND_H
#define MANDELBROT_CHANGEZOOMCOMMAND_H

#include "../command.h"

class ChangeZoomCommand : public Command
{
public:
    ChangeZoomCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    coord_t _amount;
};

#endif //MANDELBROT_CHANGEZOOMCOMMAND_H
