#ifndef MANDELBROT_SETPOSITIONCOMMAND_H
#define MANDELBROT_SETPOSITIONCOMMAND_H


#include "../command.h"

class SetPositionCommand : public Command
{
public:
    SetPositionCommand(coord_t x, coord_t y);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    Vector2D_t _position;
    Vector2D_t _oldPosition;
};


#endif //MANDELBROT_SETPOSITIONCOMMAND_H
