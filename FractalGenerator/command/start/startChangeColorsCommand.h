#ifndef MANDELBROT_STARTCHANGECOLORSCOMMAND_H
#define MANDELBROT_STARTCHANGECOLORSCOMMAND_H


#include "../../controller/controller.h"
#include "../command.h"
#include "../../colors.h"

class StartChangeColorsCommand : public Command
{
public:
    StartChangeColorsCommand(float_color_t deltaColor);

    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    float_color_t _deltaColor;
};


#endif //MANDELBROT_STARTCHANGECOLORSCOMMAND_H
