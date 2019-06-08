#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <atomic>
#include "fractalRenderer.h"
#include "../colors.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <chrono>  // for high_resolution_clock
#include <direct.h> // _mkdir
#include <thread>
#include <map>


FractalRenderer::FractalRenderer(Fractal &fractal, const std::string &folderName) :
    _fractal(fractal),
    _folderName(std::string("C:/Users/matyi/Pictures/fractal/") + folderName + "_" + std::to_string(time(nullptr))),
    _histogram(true),
    _blendMode(NO_ALPHA),
    _traceMode(DISABLE),
    _renderedImage(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC4)
{
}

void FractalRenderer::setBlendMode(BlendMode blendMode)
{
    _blendMode = blendMode;
}

void FractalRenderer::setTraceMode(TraceMode traceMode)
{
    _traceMode = traceMode;
}

bool FractalRenderer::getSaveImage() const
{
    return _saveImage;
}

void FractalRenderer::setSaveImage(bool saveImage)
{
    _saveImage = saveImage;
}

void FractalRenderer::setEnableRender(bool enable)
{
    _enableRender = enable;
}

void FractalRenderer::setFadeFactor(coord_t fadeFactor)
{
    _fadeFactor = fadeFactor;
}

void FractalRenderer::invalidate()
{
    _isValid = false;
}

void FractalRenderer::render()
{
    if (_isValid)
        return;

    uint64_t calculateTime = 0;
    uint64_t colorTime = 0;
    uint64_t saveTime = 0;
    uint64_t morphTime = 0;

    if (_enableRender) {
        cv::Mat fractalValues(_renderedImage.rows, _renderedImage.cols, CV_32FC1);

        calculateTime = calculateFractalValues(fractalValues);

        colorTime = colorPixels(fractalValues, _renderedImage);

        morphTime = morphImage(_renderedImage);

        if (_saveImage) {
            saveTime = saveImage(_renderedImage);
        }
    }

    cv::Mat copyWithText;
    _renderedImage.copyTo(copyWithText);

    drawInfoText(copyWithText, calculateTime, colorTime, saveTime);
    drawGreenCrosshair(copyWithText);

    cv::imshow(_fractal.getFractalName(), copyWithText);

    _iterationN++;

    _isValid = true;
}

uint64_t FractalRenderer::calculateFractalValues(cv::OutputArray fractalValues)
{
    const auto start = std::chrono::high_resolution_clock::now();

    cv::Mat resultMat = fractalValues.getMat();

    cv::parallel_for_(cv::Range(0, SCREEN_WIDTH * SCREEN_HEIGHT), [&](const cv::Range& range) {
        for (int r = range.start; r < range.end; r++) {
            int x = r % SCREEN_WIDTH;
            int y = r / SCREEN_WIDTH;

            fractal_value_t n = _fractal.getFractalValue(x, SCREEN_WIDTH, y, SCREEN_HEIGHT);
            resultMat.at<fractal_value_t>(y, x) = n;
        }
    });

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

uint64_t FractalRenderer::colorPixels(cv::InputArray fractalValues, cv::InputOutputArray renderedImage)
{
    const auto start = std::chrono::high_resolution_clock::now();

    if (_traceMode == DISABLE) {
        renderedImage.getMat() = cv::Scalar{ 0,0,0,0 };
    } else if (_traceMode == FADE_ALL) {
        renderedImage.getMat() *= _fadeFactor;
    }

    cv::Mat pixelValues(fractalValues.getMat());
    if (_histogram) {
        equalizeByHistogram(pixelValues);
    }

    pixelValues.convertTo(pixelValues, CV_8UC3, 255.0 / _fractal.getMaxN());

    mapGreyScaleImage(pixelValues);
    cv::cvtColor(pixelValues, pixelValues, cv::COLOR_BGR2BGRA);

    if (_blendMode == NO_ALPHA) {
        renderedImage.assign(pixelValues);
    } else if (_blendMode == EPILEPSY) {
        double alpha = 0.5;

        cv::Mat renderedImageMat = renderedImage.getMat();
        pixelValues.forEach<uint8_color_t>([&](uint8_color_t &pixel, const int position[]) {
            uint8_color_t &oldPixel = renderedImageMat.at<uint8_color_t>(position[0], position[1]);
            oldPixel[0] += (pixel[0] * alpha);            
            oldPixel[1] += (pixel[1] * alpha);
            oldPixel[2] += (pixel[2] * alpha);
            oldPixel[3] = pixel[3];
        });
    } else if (_blendMode == SMOOTH) {
        double alpha = 0.5;
        double oldAlpha = 1 - alpha;

        cv::addWeighted(renderedImage, oldAlpha, pixelValues, alpha, 1, renderedImage);
    } else if (_blendMode == SATURATED) {
        double alpha = 0.5;

        renderedImage.getMat() += (pixelValues * alpha);
    }

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

void FractalRenderer::equalizeByHistogram(cv::InputOutputArray fractalValues)
{
    const int maxN = (int)(_fractal.getMaxN() + 1);

    int histSize = maxN;
    float range[] = { 0, (float)maxN }; // the upper boundary is exclusive
    const float *histRange = { range };
    int channels[] = { 0 };

    cv::Mat hist;
    cv::calcHist(&fractalValues.getMat(), 1, channels, cv::Mat(), hist, 1, &histSize, &histRange, true, false);
    hist.convertTo(hist, CV_16UC1);

    // Calculate hue lookup table
    std::map<uint16_t, fractal_value_t> nToHue;
    fractal_value_t totalHue = 0;
    for (int k = 0; k < maxN; ++k) {
        totalHue += hist.at<uint16_t>(k);
        nToHue[k] = totalHue;
    }
    const double rec_totalHue = maxN / totalHue;
    for (int k = 0; k < maxN; ++k) {
        nToHue[k] *= rec_totalHue;
    }

    cv::parallel_for_(cv::Range(0, SCREEN_WIDTH*SCREEN_HEIGHT), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            const unsigned int x = i % TEXTURE_WIDTH;
            const unsigned int y = i / TEXTURE_WIDTH;

            fractal_value_t &n = fractalValues.getMat().at<fractal_value_t>(y, x);
            n = nToHue[n];
        }
    });
}

uint64_t FractalRenderer::morphImage(cv::InputOutputArray image)
{
    const auto start = std::chrono::high_resolution_clock::now();

    // TODO

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

uint64_t FractalRenderer::saveImage(cv::InputArray image)
{
    const auto start = std::chrono::high_resolution_clock::now();

    static bool folderExists = false;
    if (!folderExists) {
        _mkdir(_folderName.c_str());
        folderExists = true;
    }

    std::string filePath = _folderName + "/" + std::to_string(_iterationN) + ".png";

    cv::imwrite(filePath, image);

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

void FractalRenderer::drawInfoText(cv::InputOutputArray result, uint64_t calculateTime, uint64_t colorTime, uint64_t saveTime)
{
    std::stringstream ss;
    // ss << _fractal.getFractalName() << std::endl;

    ss << "X: " << std::setprecision(15) << _fractal.getXCenter() << std::endl;
    ss << "Y: " << std::setprecision(15) << _fractal.getYCenter() << std::endl;
    ss << "Zoom: " << _fractal.getZoom() << std::endl;
    ss << "Resolution: " << _fractal.getMaxN() << std::endl;
    ss << "Rotation: " << _fractal.getRotAngle() << std::endl;
    ss << "Render time: " << calculateTime + colorTime + saveTime << " ms (" << calculateTime << " + " << colorTime << " + "
        << saveTime << ")" << std::endl;
    ss << "Histogram enabled: " << _histogram << std::endl;
    ss << "Blend mode: " << to_string(_blendMode) << std::endl;
    ss << "Trace mode: " << to_string(_traceMode) << std::endl;
    if (_saveImage) {
        ss << "Image number " << _iterationN - 1 << std::endl;
    }

    std::string line;
    int y = 25;
    while (!std::getline(ss, line).eof()) {
        cv::putText(result, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, 0, 2);
        cv::putText(result, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, { 255, 255, 255 });
        y += 25;
    }
}

void FractalRenderer::drawGreenCrosshair(cv::InputOutputArray result)
{
    const cv::Scalar colorGreen = cv::Scalar(50, 255, 50);

    cv::line(result,
        { TEXTURE_WIDTH / 2 - 10, TEXTURE_HEIGHT / 2 - 10 },
        { TEXTURE_WIDTH / 2 + 10, TEXTURE_HEIGHT / 2 + 10 },
        colorGreen, 1, cv::LineTypes::LINE_AA);

    cv::line(result,
        { TEXTURE_WIDTH / 2 - 10, TEXTURE_HEIGHT / 2 + 10 },
        { TEXTURE_WIDTH / 2 + 10, TEXTURE_HEIGHT / 2 - 10 },
        colorGreen, 1, cv::LineTypes::LINE_AA);
}


std::string to_string(BlendMode blendMode)
{
    switch (blendMode) {
        case NO_ALPHA:
            return "NO ALPHA";
        case SMOOTH:
            return "SMOOTH";
        case EPILEPSY:
            return "EPILEPSY";
        case SATURATED:
            return "SATURATED";
    }
    throw std::invalid_argument("blendMode");
}

std::string to_string(TraceMode traceMode)
{
    switch (traceMode) {
        case DISABLE:
            return "DISABLE";
        case PERSIST:
            return "PERSIST";
        case FADE_FILLED:
            return "FADE_FILLED";
        case FADE_ALL:
            return "FADE_ALL";
    }
    throw std::invalid_argument("traceMode");
}