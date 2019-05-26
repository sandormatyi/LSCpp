#ifndef MANDELBROT_SETROTATIONCOMMAND_H
#define MANDELBROT_SETROTATIONCOMMAND_H


#include "../command.h"

class SetRotationCommand : public Command
{
public:
    SetRotationCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    coord_t _oldAmount;
    coord_t _amount;
};


#endif //MANDELBROT_SETROTATIONCOMMAND_H
