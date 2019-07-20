#include "UIController.h"

UIController::UIController(Fractal &fractal, FractalRenderer &renderer) :
    Controller(fractal, renderer)
{
}

UIController::~UIController()
{
    while (!_commandQueue.empty()) {
        Command *c = _commandQueue.front();
        _commandQueue.pop();
        delete c;
    }
}

void UIController::addCommand(Command* command)
{
    _commandQueue.push(command);
}

void UIController::executeAll()
{
    while (!_commandQueue.empty()) {
        Command *c = _commandQueue.front();
        _commandQueue.pop();
        c->executeOnce(*this);
        delete c;
    }
}