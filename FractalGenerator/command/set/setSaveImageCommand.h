#ifndef MANDELBROT_SETSAVEIMAGECOMMAND_H
#define MANDELBROT_SETSAVEIMAGECOMMAND_H


#include "../command.h"

class SetSaveImageCommand : public Command
{
public:
    SetSaveImageCommand(const std::string &saveImageFolder);
    void executeOnce(Controller &c) override;
    void undo(Controller &c) override;
private:
    std::string _saveImageFolder;
};


#endif //MANDELBROT_SETSAVEIMAGECOMMAND_H
