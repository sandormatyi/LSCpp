#pragma once

#include "controller.h"
#include <queue>
#include "../command/command.h"

class UIController : public Controller
{
public:
    UIController(Fractal &fractal, FractalRenderer &renderer);
    ~UIController();

    void addCommand(Command* command);
    void executeAll();

private:
    std::queue<Command*> _commandQueue;
};

