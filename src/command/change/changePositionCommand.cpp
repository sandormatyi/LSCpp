#include "changePositionCommand.h"

ChangePositionCommand::ChangePositionCommand(coord_t x, coord_t y)
{
    _vector = {x, y};
}

void ChangePositionCommand::executeOnce(Controller &c)
{
    c.getFractal().move(_vector);
}

void ChangePositionCommand::undo(Controller &c)
{
    Vector2D_t oppositeVector{
            -_vector.x,
            -_vector.y};

    c.getFractal().move(oppositeVector);
}