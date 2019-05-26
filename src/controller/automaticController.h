#ifndef MANDELBROT_AUTOMATICCONTROLLER_H
#define MANDELBROT_AUTOMATICCONTROLLER_H


#include <fstream>
#include <map>
#include <vector>
#include "controller.h"
#include "../command/command.h"

struct ScoreData {
    std::string title;
    double bpm;
    coord_t x;
    coord_t y;
    coord_t zoom;
    coord_t maxN;
};

struct BeatData {
    std::vector<Command*> commands;
};

struct CommandData {
    std::vector<BeatData> beats;
    unsigned int currentBeatIndex = 0;
};

class AutomaticController : public Controller
{
public:
    AutomaticController(Fractal &fractal, FractalRenderer &renderer, const std::string &fileName);
    ~AutomaticController();

    void initialize();

    void step();

    void executeNextBeat();
    void undoLastBeat();

private:
    ScoreData readScoreData(std::ifstream &inputFile);
    CommandData readCommandData(std::ifstream &inputFile);

    ScoreData _scoreData;
    CommandData _commandData;
    unsigned int _currentFrameNumber = 0;
};


#endif //MANDELBROT_AUTOMATICCONTROLLER_H
