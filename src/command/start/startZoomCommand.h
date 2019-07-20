#ifndef MANDELBROT_STARTZOOMCOMMAND_H
#define MANDELBROT_STARTZOOMCOMMAND_H


#include "../command.h"

class StartZoomCommand : public Command
{
public:
    StartZoomCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    coord_t _amount;
};


#endif //MANDELBROT_STARTZOOMCOMMAND_H
