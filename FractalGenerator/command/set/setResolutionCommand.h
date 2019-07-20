#ifndef MANDELBROT_SETRESOLUTIONCOMMAND_H
#define MANDELBROT_SETRESOLUTIONCOMMAND_H


#include "../command.h"

class SetResolutionCommand : public Command
{
public:
    SetResolutionCommand(fractal_value_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    fractal_value_t _amount;
    fractal_value_t _oldAmount;
};


#endif //MANDELBROT_SETRESOLUTIONCOMMAND_H
