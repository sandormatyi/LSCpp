#ifndef MANDELBROT_CHANGEPOSITIONCOMMAND_H
#define MANDELBROT_CHANGEPOSITIONCOMMAND_H

#include "../command.h"

class ChangePositionCommand : public Command
{
public:
    ChangePositionCommand(coord_t x, coord_t y);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    Vector2D_t _vector;
};

#endif //MANDELBROT_CHANGEPOSITIONCOMMAND_H
