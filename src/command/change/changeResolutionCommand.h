#ifndef MANDELBROT_CHANGERESOLUTIONCOMMAND_H
#define MANDELBROT_CHANGERESOLUTIONCOMMAND_H

#include "../command.h"

class ChangeResolutionCommand : public Command
{
public:
    ChangeResolutionCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    coord_t _amount;
    coord_t _actualAmount;
};

#endif //MANDELBROT_CHANGERESOLUTIONCOMMAND_H
