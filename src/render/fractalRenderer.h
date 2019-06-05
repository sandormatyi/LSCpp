#ifndef MANDELBROT_FRACTALRENDERER_H
#define MANDELBROT_FRACTALRENDERER_H

#include <string>
#include <ctime>
#include <atomic>
#include "../fractal/fractal.h"
#include "renderParams.h"
#include "opencv2/core/mat.hpp"
#include "../colors.h"

enum BlendMode
{
    NO_ALPHA = 0,
    SMOOTH = 1,
    EPILEPSY = 2
};

enum ColorMode
{
    LINEAR = 0,
    HISTOGRAM = 1
};

enum TraceMode
{
    DISABLE = 0,
    PERSIST = 1,
    FADE_FILLED = 2,
    FADE_ALL = 3
};

std::string to_string(BlendMode blendMode);
std::string to_string(ColorMode colorMode);
std::string to_string(TraceMode traceMode);


class FractalRenderer
{
public:
    FractalRenderer(Fractal &fractal, cv::InputOutputArray matrix, const std::string &folderName);

    void invalidate();
    void render();

    void setEnableRender(bool enable);

    bool getSaveImage() const;

    void setFadeFactor(coord_t fadeFactor);
    void setSaveImage(bool saveImage);
    void setBlendMode(BlendMode blendMode);
    void setColorMode(ColorMode colorMode);
    void setTraceMode(TraceMode traceMode);

private:
    uint64_t calculateFractalValues(cv::OutputArray result);
    uint64_t colorPixels(cv::InputArray fractalValues);
    uint64_t saveImage();

    void drawInfoText(uint64_t calculateTime, uint64_t colorTime, uint64_t saveTime);
    void drawGreenCrosshair();

    void colorByHistogram(cv::InputArray fractalValues);
    void colorLinear(cv::InputArray fractalValues);
    void colorPixel(const SDL_Color &c, int x, int y);

private:
    Fractal &_fractal;
    cv::InputOutputArray _matrix;

    bool _isValid = false;
    bool _enableRender = true;
    bool _saveImage = false;
    const std::string _folderName;

    coord_t _fadeFactor = 0.75;
    BlendMode _blendMode = NO_ALPHA;
    ColorMode _colorMode = LINEAR;
    TraceMode _traceMode = DISABLE;

    int _iterationN = 1;
};


#endif //MANDELBROT_FRACTALRENDERER_H
