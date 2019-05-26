#ifndef MANDELBROT_SETSAVEIMAGECOMMAND_H
#define MANDELBROT_SETSAVEIMAGECOMMAND_H


#include "../command.h"

class SetSaveImageCommand : public Command
{
public:
    SetSaveImageCommand(bool saveImage);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    bool _saveImage;
    bool _previousValue;
};


#endif //MANDELBROT_SETSAVEIMAGECOMMAND_H
