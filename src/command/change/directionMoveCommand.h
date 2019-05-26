#ifndef MANDELBROT_DIRECTIONMOVECOMMAND_H
#define MANDELBROT_DIRECTIONMOVECOMMAND_H

#include "../command.h"

class DirectionMoveCommand : public Command
{
public:
    DirectionMoveCommand(Direction direction, coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    Direction _direction;
    coord_t _amount;
};

#endif //MANDELBROT_DIRECTIONMOVECOMMAND_H
