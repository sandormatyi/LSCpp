#include "setSaveImageCommand.h"

SetSaveImageCommand::SetSaveImageCommand(bool saveImage) : _saveImage(saveImage) {}

void SetSaveImageCommand::executeOnce(Controller &c)
{
    _previousValue = c.getFractalRenderer().getSaveImage();
    c.getFractalRenderer().setSaveImage(_saveImage);
}

void SetSaveImageCommand::undo(Controller &c)
{
    c.getFractalRenderer().setSaveImage(_previousValue);
}
