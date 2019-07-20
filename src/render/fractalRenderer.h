#ifndef MANDELBROT_FRACTALRENDERER_H
#define MANDELBROT_FRACTALRENDERER_H

#include <string>
#include <ctime>
#include <atomic>
#include <opencv2/core.hpp>
#include "../fractal/fractal.h"
#include "renderParams.h"
#include "../types.h"


std::string to_string(BlendMode blendMode);
std::string to_string(TraceMode traceMode);


class FractalRenderer
{
public:
    FractalRenderer(Fractal &fractal, const std::string &folderName);

    void invalidate();
    void render();

    bool getSaveImage() const;

    void setFadeFactor(coord_t fadeFactor);
    void setEnableRender(bool enable);
    void setSaveImage(bool saveImage);
    void setBlendMode(BlendMode blendMode);
    void setTraceMode(TraceMode traceMode);

private:
    uint64_t calculateFractalValues(cv::OutputArray fractalValues);
    void equalizeByHistogram(cv::InputOutputArray fractalValues);
    uint64_t colorPixels(cv::InputArray fractalValues, cv::InputOutputArray renderedImage);
    uint64_t morphImage(cv::InputOutputArray image);
    uint64_t saveImage(cv::InputArray image);

    void drawInfoText(cv::InputOutputArray result, uint64_t calculateTime, uint64_t colorTime, uint64_t saveTime);
    void drawGreenCrosshair(cv::InputOutputArray result);

private:
    Fractal &_fractal;

    bool _isValid = false;
    bool _enableRender = true;
    bool _saveImage = false;
    const std::string _folderName;

    coord_t _fadeFactor = 0.75;
    bool _histogram;
    BlendMode _blendMode;
    TraceMode _traceMode;

    unsigned int _iterationN = 1;
    cv::Mat _renderedImage;
};


#endif //MANDELBROT_FRACTALRENDERER_H
