#ifndef MANDELBROT_COMMAND_H
#define MANDELBROT_COMMAND_H


#include <string>
#include "../fractal/mandelbrot.h"
#include "../controller/controller.h"


class Command
{
public:
    virtual ~Command() {};
    virtual void executeOnce(Controller &c) = 0;
    virtual void undo(Controller &c) = 0;
};


#endif //MANDELBROT_COMMAND_H
