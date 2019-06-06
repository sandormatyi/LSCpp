#include <iostream>
#include <sstream>
#include <vector>
#include "fractal/mandelbrot.h"
#include "render/renderedText.h"
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

/*
//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window *gWindow = nullptr;

//The window renderer
SDL_Renderer *gRenderer = nullptr;

bool init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Create window
    gWindow = SDL_CreateWindow("Mandelbrot fractal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }


    //Create renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //bitmap = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    initColors();
    initSDLTTF();

    //Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}

void close()
{
    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    SDL_Quit();
}
*/


int main(int argc, char *args[])
{
    initColors();

    Mandelbrot m(-0.302544, -0.043626, 4.45019, 464);
    //ExternalImage m(0, 0, 1, 64, "budi_6.png", SDL_GetWindowSurface(gWindow));

    cv::Mat img(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC4);
    FractalRenderer fractalRenderer(m, img, "Mosolypersely");

    AutomaticController automaticController(m, fractalRenderer, "testCommandFile.mnd");
    automaticController.initialize();

    KeyboardController keyboardController(m, fractalRenderer, 1.05, 0.05, 1, 2, {1, 2, 3, 0});


    //While application is running
    while (!Controller::isQuitFlagSet()) {
        int key = cv::waitKeyEx(1);
        if (key > 0) {
            std::bitset<32> binary(key);
            std::cout << key << " pressed (" << (char)key << " - " << binary.to_string() << ") " << std::endl;
        }
        if (key > 0 && keyboardController.processKeyboardInput(key)) {
            fractalRenderer.invalidate();
        }
        if (keyboardController.doAutomaticTransformations()) {
            fractalRenderer.invalidate();
        }
        if (scoreEnabled) {
            automaticController.step();
            automaticController.doAutomaticTransformations();
            fractalRenderer.invalidate();
        }

        fractalRenderer.render();
    }

    //Free resources and close SDL

    return 0;
}




