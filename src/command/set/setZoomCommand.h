#ifndef MANDELBROT_SETZOOMCOMMAND_H
#define MANDELBROT_SETZOOMCOMMAND_H


#include "../command.h"

class SetZoomCommand : public Command
{
public:
    SetZoomCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    coord_t _amount;
    coord_t _oldAmount;
};


#endif //MANDELBROT_SETZOOMCOMMAND_H
