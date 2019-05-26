#ifndef MANDELBROT_LOGCOMMAND_H
#define MANDELBROT_LOGCOMMAND_H


#include "command.h"

class LogCommand : public Command
{
public:
    LogCommand(const std::string &message);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;

private:
    std::string _message;
};


#endif //MANDELBROT_LOGCOMMAND_H
