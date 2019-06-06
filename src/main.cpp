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
    while (!automaticController.isQuitFlagSet()) {
    //    //Handle events on queue
    //    while (SDL_PollEvent(&e) != 0) {
    //        const uint8_t *keys = SDL_GetKeyboardState(nullptr);
    //        bool isShiftPressed = keys[SDL_GetScancodeFromKey(SDLK_LSHIFT)];

    //        //User requests quit
    //        if (e.type == SDL_KEYDOWN) {
    //            switch (e.key.keysym.sym) {
    //                case SDLK_ESCAPE:
    //                    automaticController.setQuitFlag();
    //                    break;
    //                case SDLK_r:
    //                    fractalRenderer.setSaveImage(!fractalRenderer.getSaveImage());
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_1:
    //                    fractalRenderer.setBlendMode(NO_ALPHA);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_2:
    //                    fractalRenderer.setBlendMode(SMOOTH);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_3:
    //                    fractalRenderer.setBlendMode(EPILEPSY);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_F1:
    //                    fractalRenderer.setTraceMode(DISABLE);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_F2:
    //                    fractalRenderer.setTraceMode(PERSIST);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_F3:
    //                    fractalRenderer.setTraceMode(FADE_FILLED);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_F4:
    //                    fractalRenderer.setTraceMode(FADE_ALL);
    //                    fractalRenderer.invalidate();
    //                    break;
    //                case SDLK_SPACE:
    //                    scoreEnabled = !scoreEnabled;
    //                    break;
    //                case SDLK_BACKSPACE:
    //                    automaticController.undoLastBeat();
    //                    fractalRenderer.invalidate();
    //                    break;
    //                default:
    //                    if (keyboardController.processKeyboardInput(e.key.keysym.sym, isShiftPressed)) {
    //                        fractalRenderer.invalidate();
    //                    }
    //                    break;
    //            }
    //        } else if (e.type == SDL_QUIT) {
    //            automaticController.setQuitFlag();
    //        }
    //    }
        int key = cv::waitKeyEx(1);
        std::bitset<32> binary(key);
        std::cout << key << " pressed. (" << binary.to_string() << ") " << std::endl;
        if (key > 0 && keyboardController.processKeyboardInput(key, false)) {
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




