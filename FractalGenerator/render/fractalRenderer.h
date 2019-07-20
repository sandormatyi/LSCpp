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
    FractalRenderer(Fractal &fractal, int screenWidth, int screenHeight);

    void invalidate();
    void render();
    void setScreenSize(int width, int height);

    bool getSaveImage() const;
    void enableSaveImage(const std::string &folderName);
    void disableSaveImage();

    void setFadeFactor(double fadeFactor);
    void setAlpha(double alpha);
    void setEnableHistogram(bool enable);
    void setEnableRender(bool enable);
    void setBlendMode(BlendMode blendMode);
    void setTraceMode(TraceMode traceMode);

    std::string getInfoText();

private:
    uint64_t calculateFractalValues(cv::OutputArray fractalValues);
    void equalizeByHistogram(cv::InputOutputArray fractalValues);
    uint64_t colorPixels(cv::InputArray fractalValues, cv::InputOutputArray renderedImage);
    uint64_t morphImage(cv::InputOutputArray image);
    uint64_t saveImage(cv::InputArray image);

    void drawInfoText(cv::InputOutputArray result, uint64_t calculateTime, uint64_t colorTime, uint64_t saveTime);
    void drawGreenCrosshair(cv::InputOutputArray result);

private:
    int _screenWidth;
    int _screenHeight;
    Fractal &_fractal;

    bool _isValid = false;
    bool _enableRender = true;
    std::string _saveFolderName;

    double _fadeFactor = 0.75;
    double _alpha = 0.5;
    bool _histogram;
    BlendMode _blendMode;
    TraceMode _traceMode;

    uint64_t _lastCalculateTime = 0;
    uint64_t _lastColorTime = 0;
    uint64_t _lastSaveTime = 0;
    uint64_t _lastMorphTime = 0;

    unsigned int _iterationN = 1;
    std::mutex _mutex;
    cv::Mat _renderedImage;
};


#endif //MANDELBROT_FRACTALRENDERER_H
