// FractalGenerator.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
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
#include "command/allCommands.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <bitset>
#include <comutil.h>
#include "controller/keys.h"

#define FractalExport extern "C" __declspec (dllexport) 


static Fractal *fractal = nullptr;
static FractalRenderer *renderer = nullptr;
static KeyboardController *controller = nullptr;
static std::string infoString;

BSTR ANSItoBSTR(const char* input);

FractalExport
void Init(int screenWidth, int screenHeight)
{
    initColors();

    fractal = new Mandelbrot(0, 0, 1, 1024);
    renderer = new FractalRenderer(*fractal, screenWidth, screenHeight);
    controller = new KeyboardController(*fractal, *renderer, 1.05, 0.05, 1, 2, { 1, 2, 3, 0 });
}

FractalExport
void DeInit()
{
    cv::destroyAllWindows();

    delete controller;
    delete renderer;
    delete fractal;
}

FractalExport
void Render()
{
    controller->executeAll();
    renderer->invalidate();
    renderer->render();
}

FractalExport
void ChangeScreenSize(int screenWidth, int screenHeight)
{
    renderer->setScreenSize(screenWidth, screenHeight);
}

FractalExport
BSTR GetInfoText()
{
    infoString = renderer->getInfoText();
    return ANSItoBSTR(infoString.c_str());
}

FractalExport
void SetControlParams(double zoomSpeed, double rotation, fractal_value_t deltaResolution)
{
    controller->setZoomSpeed(zoomSpeed);
    controller->setDeltaRot(rotation);
    controller->setDeltaN(deltaResolution);
}

FractalExport
void SetRenderParams(BlendMode blendMode, TraceMode traceMode, int enableHistogram, double fadeFactor, double alpha)
{
    renderer->setBlendMode(blendMode);
    renderer->setTraceMode(traceMode);
    renderer->setEnableHistogram(enableHistogram != 0);
    renderer->setFadeFactor(fadeFactor);
    renderer->setAlpha(alpha);
}

FractalExport
void SetPosition(double x, double y, double zoom, double rotation, fractal_value_t resolution)
{
    fractal->setXCenter(x);
    fractal->setYCenter(y);
    fractal->setMaxN(resolution);
    fractal->setRotAngle(rotation);
    fractal->setZoom(zoom);
}

FractalExport
bool ProcessKeyboardInput(Keys key, bool shift)
{
    return controller->processKeyboardInput(key, shift);
}

FractalExport
bool Step()
{
    return controller->doAutomaticTransformations();
}

FractalExport
void ChangeColorMap(int colorMap)
{
    changeColorMap(colorMap);
}

BSTR ANSItoBSTR(const char* input)
{
    BSTR result = NULL;
    int lenA = lstrlenA(input);
    int lenW = ::MultiByteToWideChar(CP_ACP, 0, input, lenA, NULL, 0);
    if (lenW > 0) {
        result = ::SysAllocStringLen(0, lenW);
        ::MultiByteToWideChar(CP_ACP, 0, input, lenA, result, lenW);
    }
    return result;
}