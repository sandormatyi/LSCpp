#include "setSaveImageCommand.h"
#include "../../controller/controller.h"
#include "../../render/fractalRenderer.h"

SetSaveImageCommand::SetSaveImageCommand(const std::string &saveImageFolder) :
    _saveImageFolder(saveImageFolder)
{
}

void SetSaveImageCommand::executeOnce(Controller &c)
{
    if (_saveImageFolder.empty()) {
        c.getFractalRenderer().disableSaveImage();
    } else {
        c.getFractalRenderer().enableSaveImage(_saveImageFolder);
    }
}

void SetSaveImageCommand::undo(Controller &c)
{
}
