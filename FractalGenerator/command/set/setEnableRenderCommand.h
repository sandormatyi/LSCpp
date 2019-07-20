#ifndef MANDELBROT_SETENABLERENDERCOMMAND_H
#define MANDELBROT_SETENABLERENDERCOMMAND_H


#include "../command.h"

class SetEnableRenderCommand : public Command
{
public:
    SetEnableRenderCommand(bool enable);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    bool _enable;
    bool _previousValue;
};


#endif //MANDELBROT_SETENABLERENDERCOMMAND_H
