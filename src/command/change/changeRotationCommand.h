#ifndef MANDELBROT_CHANGEROTATIONCOMMAND_H
#define MANDELBROT_CHANGEROTATIONCOMMAND_H

#include "../command.h"

class ChangeRotationCommand : public Command
{
public:
    ChangeRotationCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    coord_t _amount;
};

#endif //MANDELBROT_CHANGEROTATIONCOMMAND_H
