#ifndef MANDELBROT_COMMAND_H
#define MANDELBROT_COMMAND_H

#include "../types.h"

class Controller;

class Command
{
public:
    virtual ~Command() {};
    virtual void executeOnce(Controller &c) = 0;
    virtual void undo(Controller &c) = 0;
};


#endif //MANDELBROT_COMMAND_H
