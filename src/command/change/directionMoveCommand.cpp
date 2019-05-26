#include "directionMoveCommand.h"

DirectionMoveCommand::DirectionMoveCommand(Direction direction, coord_t amount) : _direction(direction),
                                                                                  _amount(amount) {}

void DirectionMoveCommand::executeOnce(Controller &c)
{
    c.getFractal().move(_direction, _amount);
}

void DirectionMoveCommand::undo(Controller &c)
{
    c.getFractal().move(_direction, -_amount);
}