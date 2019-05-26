#ifndef MANDELBROT_STARTCHANGERESOLUTIONCOMMAND_H
#define MANDELBROT_STARTCHANGERESOLUTIONCOMMAND_H


#include "../command.h"

class StartChangeResolutionCommand : public Command
{
public:
    StartChangeResolutionCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    coord_t _amount;

};

#endif //MANDELBROT_STARTCHANGERESOLUTIONCOMMAND_H
