#ifndef MANDELBROT_CHANGECOLORSCOMMAND_H
#define MANDELBROT_CHANGECOLORSCOMMAND_H

#include "../command.h"

class ChangeColorsCommand : public Command
{
public:
    ChangeColorsCommand(float_color_t deltaColor);

    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    float_color_t _deltaColor;
};


#endif //MANDELBROT_CHANGECOLORSCOMMAND_H
