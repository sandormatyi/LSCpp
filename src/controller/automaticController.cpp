#include <sstream>
#include <cmath>
#include "automaticController.h"
#include "../command/change/changeZoomCommand.h"
#include "../command/change/changeRotationCommand.h"
#include "../command/change/changeResolutionCommand.h"
#include "../command/change/changePositionCommand.h"
#include "../command/change/changeColorsCommand.h"
#include "../command/start/startChangeColorsCommand.h"
#include "../command/start/startZoomCommand.h"
#include "../command/start/startRotateCommand.h"
#include "../command/start/startChangeResolutionCommand.h"
#include "../command/stop/stopZoomCommand.h"
#include "../command/stop/stopRotateCommand.h"
#include "../command/stop/stopChangeColorsCommand.h"
#include "../render/renderParams.h"
#include "../command/set/setTraceModeCommand.h"
#include "../command/set/setBlendModeCommand.h"
#include "../command/set/setZoomCommand.h"
#include "../command/set/setRotationCommand.h"
#include "../command/set/setResolutionCommand.h"
#include "../command/set/setPositionCommand.h"
#include "../command/set/setSaveImageCommand.h"
#include "../command/stop/stopChangeResolutionCommand.h"
#include "../command/stop/stopProgramCommand.h"
#include "../command/set/setEnableRenderCommand.h"
#include "../command/set/setFadeFactorCommand.h"
#include "../command/logCommand.h"


AutomaticController::AutomaticController(Fractal &fractal, FractalRenderer &renderer, const std::string &fileName) :
        Controller(fractal, renderer)
{
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open())
        throw std::invalid_argument(fileName);

    _scoreData = readScoreData(inputFile);
    _commandData = readCommandData(inputFile);
}

AutomaticController::~AutomaticController()
{
    for (BeatData &beatData : _commandData.beats) {
        for (Command *command : beatData.commands) {
            delete (command);
        }
    }
}

ScoreData AutomaticController::readScoreData(std::ifstream &inputFile)
{
    ScoreData scoreData{};

    std::string line;
    while (std::getline(inputFile, line) && line != "-") {
        std::istringstream stringStream(line);
        std::string parameter;
        std::string value;
        stringStream >> parameter >> value;
        if (parameter == "title") {
            scoreData.title = value;
        } else if (parameter == "bpm") {
            scoreData.bpm = atof(value.c_str());
        } else if (parameter == "x") {
            scoreData.x = atof(value.c_str());
        } else if (parameter == "y") {
            scoreData.y = atof(value.c_str());
        } else if (parameter == "zoom") {
            scoreData.zoom = atof(value.c_str());
        } else if (parameter == "maxn") {
            scoreData.maxN = atof(value.c_str());
        }
    }

    return scoreData;
}

CommandData AutomaticController::readCommandData(std::ifstream &inputFile)
{
    CommandData commandData{};
    const double framesPerBeat = (FRAMES_PER_SECOND * 60) / _scoreData.bpm;
    int beatIndex = 0;
    commandData.beats.push_back({});

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream stringStream(line);
        std::string command;
        std::getline(stringStream, command, ' ');

        if (command == "#") {
            continue;
        } else if (command == "-") {
            beatIndex++;
            commandData.beats.push_back({});

            int barNumber = (beatIndex + 3) / 4;
            int beatNumber = (beatIndex + 3) % 4 + 1;
            std::stringstream ss;
            ss << barNumber << "/" << beatNumber;
            commandData.beats[beatIndex - 1].commands.push_back(new LogCommand(ss.str()));
        } else if (command == "change") {
            std::getline(stringStream, command, ' ');

            if (command == "zoom") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                commandData.beats[beatIndex].commands.push_back(new ChangeZoomCommand(amount));
            } else if (command == "rotation") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                commandData.beats[beatIndex].commands.push_back(new ChangeRotationCommand(amount));
            } else if (command == "resolution") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                commandData.beats[beatIndex].commands.push_back(new ChangeResolutionCommand(amount));
            } else if (command == "position") {
                coord_t x, y;
                stringStream >> x >> y;
                commandData.beats[beatIndex].commands.push_back(new ChangePositionCommand(x, y));
            } else if (command == "color") {
                float r, g, b, a;
                stringStream >> r >> g >> b >> a;
                commandData.beats[beatIndex].commands.push_back(
                        new ChangeColorsCommand({r, g, b, a}));
            }
        } else if (command == "set") {
            std::getline(stringStream, command, ' ');

            if (command == "zoom") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                commandData.beats[beatIndex].commands.push_back(new SetZoomCommand(amount));
            } else if (command == "rotation") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                commandData.beats[beatIndex].commands.push_back(new SetRotationCommand(amount));
            } else if (command == "resolution") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                commandData.beats[beatIndex].commands.push_back(new SetResolutionCommand(amount));
            } else if (command == "position") {
                coord_t x, y;
                stringStream >> x >> y;
                commandData.beats[beatIndex].commands.push_back(new SetPositionCommand(x, y));
            } else if (command == "fade") {
                coord_t amount;
                stringStream >> amount;
                commandData.beats[beatIndex].commands.push_back(new SetFadeFactorCommand(amount));
            } else if (command == "tracemode") {
                int traceMode;
                stringStream >> traceMode;
                commandData.beats[beatIndex].commands.push_back(new SetTraceModeCommand((TraceMode) traceMode));
            } else if (command == "blend") {
                int blendMode;
                stringStream >> blendMode;
                commandData.beats[beatIndex].commands.push_back(new SetBlendModeCommand((BlendMode) blendMode));
            } else if (command == "save") {
                bool enable;
                stringStream >> enable;
                commandData.beats[beatIndex].commands.push_back(new SetSaveImageCommand(enable));
            } else if (command == "render") {
                bool enable;
                stringStream >> enable;
                commandData.beats[beatIndex].commands.push_back(new SetEnableRenderCommand(enable));
            }
        } else if (command == "start") {
            std::getline(stringStream, command, ' ');

            if (command == "zoom") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                coord_t scaledAmount = std::pow(amount, 1 / framesPerBeat);
                commandData.beats[beatIndex].commands.push_back(new StartZoomCommand(scaledAmount));
            } else if (command == "rotation") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                coord_t scaledAmount = amount / framesPerBeat;
                commandData.beats[beatIndex].commands.push_back(new StartRotateCommand(scaledAmount));
            } else if (command == "resolution") {
                std::string param;
                std::getline(stringStream, param);
                coord_t amount = atof(param.c_str());
                coord_t scaledAmount = amount / framesPerBeat;
                commandData.beats[beatIndex].commands.push_back(new StartChangeResolutionCommand(scaledAmount));
            } else if (command == "move") {
                coord_t x, y;
                stringStream >> x >> y;
                //commandData.beats[beatIndex].commands.push_back(new ChangePositionCommand(x, y));
            } else if (command == "color") {
                float r, g, b, a;
                stringStream >> r >> g >> b >> a;
                float sR = r / framesPerBeat;
                float sG = g / framesPerBeat;
                float sB = b / framesPerBeat;
                float sA = a / framesPerBeat;
                commandData.beats[beatIndex].commands.push_back(
                        new StartChangeColorsCommand({sR, sG, sB, sA}));
            }
        } else if (command == "stop") {
            std::getline(stringStream, command, ' ');

            if (command == "zoom") {
                commandData.beats[beatIndex].commands.push_back(new StopZoomCommand());
            } else if (command == "rotation") {
                commandData.beats[beatIndex].commands.push_back(new StopRotateCommand());
            } else if (command == "resolution") {
                commandData.beats[beatIndex].commands.push_back(new StopChangeResolutionCommand());
            } else if (command == "move") {
                //commandData.beats[beatIndex].commands.push_back(new ChangePositionCommand(x, y));
            } else if (command == "color") {
                commandData.beats[beatIndex].commands.push_back(new StopChangeColorsCommand());
            }
        } else if (command == "quit") {
            commandData.beats[beatIndex].commands.push_back(new StopProgramCommand());
        } else if (command == "log") {
            std::string param;
            std::getline(stringStream, param);
            commandData.beats[beatIndex].commands.push_back(new LogCommand(param));
        }
    }

    return commandData;
}

void AutomaticController::initialize()
{
    _fractal.setXCenter(_scoreData.x);
    _fractal.setYCenter(_scoreData.y);
    _fractal.setMaxN(_scoreData.maxN);
    _fractal.setZoom(_scoreData.zoom);
}

void AutomaticController::executeNextBeat()
{
    if (_commandData.currentBeatIndex >= _commandData.beats.size())
        return;

    for (Command *c : _commandData.beats[_commandData.currentBeatIndex].commands) {
        c->executeOnce(*this);
    }
    _commandData.currentBeatIndex++;
}

void AutomaticController::undoLastBeat()
{
    if (_commandData.currentBeatIndex <= 0)
        return;

    _commandData.currentBeatIndex--;

    for (Command *c : _commandData.beats[_commandData.currentBeatIndex].commands) {
        c->undo(*this);
    }
}

void AutomaticController::step()
{
    const double framesPerBeat = (FRAMES_PER_SECOND * 60) / _scoreData.bpm;
    _currentFrameNumber++;

    if (_currentFrameNumber >= _commandData.currentBeatIndex * framesPerBeat) {
        executeNextBeat();
    }
}
