#ifndef MANDELBROT_STARTROTATECOMMAND_H
#define MANDELBROT_STARTROTATECOMMAND_H


#include "../command.h"

class StartRotateCommand : public Command
{
public:
    StartRotateCommand(coord_t amount);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    coord_t _amount;
};


#endif //MANDELBROT_STARTROTATECOMMAND_H
