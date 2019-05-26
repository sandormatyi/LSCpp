#ifndef MANDELBROT_SETFADEFACTORCOMMAND_H
#define MANDELBROT_SETFADEFACTORCOMMAND_H


#include "../command.h"

class SetFadeFactorCommand : public Command
{
public:
    SetFadeFactorCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    coord_t _amount;
};


#endif //MANDELBROT_SETFADEFACTORCOMMAND_H
