#ifndef MANDELBROT_CHANGERESOLUTIONCOMMAND_H
#define MANDELBROT_CHANGERESOLUTIONCOMMAND_H

#include "../command.h"

class ChangeResolutionCommand : public Command
{
public:
    ChangeResolutionCommand(fractal_value_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    fractal_value_t _amount;
    fractal_value_t _actualAmount;
};

#endif //MANDELBROT_CHANGERESOLUTIONCOMMAND_H
