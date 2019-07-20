#ifndef MANDELBROT_SETRESOLUTIONCOMMAND_H
#define MANDELBROT_SETRESOLUTIONCOMMAND_H


#include "../command.h"

class SetResolutionCommand : public Command
{
public:
    SetResolutionCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    coord_t _amount;
    coord_t _oldAmount;
};


#endif //MANDELBROT_SETRESOLUTIONCOMMAND_H
