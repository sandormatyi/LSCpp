/*
#include <iostream>
#include <sstream>
#include <vector>
#include "fractal/mandelbrot.h"
#include <cstring>
#include <ctime>
#include "controller/controller.h"
#include "controller/keyboardController.h"
#include "controller/automaticController.h"
#include "render/renderParams.h"
#include "colors.h"
#include "render/fractalRenderer.h"
#include "fractal/externalImage.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <bitset>

bool scoreEnabled = false;


int main(int argc, char *args[])
{
    std::cout << cv::getBuildInformation() << std::endl;

    initColors();

    Mandelbrot m(-0.815276444999583, -0.1759531681, 10, 1024);
    //ExternalImage m(0, 0, 1, 64, "budi_6.png");

    FractalRenderer fractalRenderer(m, "Mosolypersely");

    AutomaticController automaticController(m, fractalRenderer, "testCommandFile.mnd");
    //automaticController.initialize();

    KeyboardController keyboardController(m, fractalRenderer, 1.05, 0.05, 1, 2, {1, 2, 3, 0});


    //While application is running
    while (!Controller::isQuitFlagSet()) {
        fractalRenderer.render();

        int key = cv::waitKeyEx(10);
        if (key > 0) {
            std::bitset<32> binary(key);
            std::cout << (char)key << " (" << key << ") " << std::endl;
            if (keyboardController.processKeyboardInput(key)) {
                fractalRenderer.invalidate();
            }
        }

        if (keyboardController.doAutomaticTransformations()) {
            fractalRenderer.invalidate();
        }

        if (scoreEnabled) {
            automaticController.step();
            automaticController.doAutomaticTransformations();
            fractalRenderer.invalidate();
        }
    }

    //Free resources and exit the application
    cv::destroyAllWindows();

    return 0;
}
*/