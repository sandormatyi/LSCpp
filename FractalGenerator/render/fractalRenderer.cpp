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


FractalRenderer::FractalRenderer(Fractal &fractal, int screenWidth, int screenHeight) :
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _fractal(fractal),
    _saveFolderName(),
    _histogram(true),
    _blendMode(NO_ALPHA),
    _traceMode(DISABLE),
    _renderedImage(screenHeight, screenWidth, CV_8UC4)
{
    cv::namedWindow(_fractal.getFractalName(), 0);
    cv::setWindowProperty(_fractal.getFractalName(), cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
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
    return !_saveFolderName.empty();
}

void FractalRenderer::enableSaveImage(const std::string& folderName)
{
    _saveFolderName = folderName;
}

void FractalRenderer::disableSaveImage()
{
    _saveFolderName.clear();
}

void FractalRenderer::setEnableRender(bool enable)
{
    _enableRender = enable;
}

void FractalRenderer::setFadeFactor(double fadeFactor)
{
    _fadeFactor = fadeFactor;
}

void FractalRenderer::setAlpha(double alpha)
{
    _alpha = alpha;
}

void FractalRenderer::setEnableHistogram(bool enable)
{
    _histogram = enable;
}

std::string FractalRenderer::getInfoText()
{
    std::stringstream ss;
    // ss << _fractal.getFractalName() << std::endl;

    ss << "X: " << std::setprecision(15) << _fractal.getXCenter() << "\r\n";
    ss << "Y: " << std::setprecision(15) << _fractal.getYCenter() << "\r\n";
    ss << "Zoom: " << _fractal.getZoom() << "\r\n";
    ss << "Resolution: " << _fractal.getMaxN() << "\r\n";
    ss << "Rotation: " << _fractal.getRotAngle() << "\r\n";
    ss << "Render time: " << _lastCalculateTime + _lastColorTime + _lastSaveTime << " ms (" << _lastCalculateTime << " + " << _lastColorTime << " + "
        << _lastSaveTime << ")" << "\r\n";
    ss << "Histogram enabled: " << _histogram << "\r\n";
    ss << "Blend mode: " << to_string(_blendMode) << "\r\n";
    ss << "Trace mode: " << to_string(_traceMode) << "\r\n";
    if (getSaveImage()) {
        ss << "Image number " << _iterationN - 1 << "\r\n";
    }

    return ss.str();
}

void FractalRenderer::invalidate()
{
    _isValid = false;
}

void FractalRenderer::render()
{
    if (_isValid)
        return;

    std::lock_guard<std::mutex> guard(_mutex);

    if (_enableRender) {
        cv::Mat fractalValues(_screenHeight, _screenWidth, CV_32FC1);

        _lastCalculateTime = calculateFractalValues(fractalValues);
        _lastColorTime = colorPixels(fractalValues, _renderedImage);
        _lastMorphTime = morphImage(_renderedImage);
        if (getSaveImage()) {
            _lastSaveTime = saveImage(_renderedImage);
        }
    }

    cv::Mat copyWithText;
    _renderedImage.copyTo(copyWithText);

    drawInfoText(copyWithText, _lastCalculateTime, _lastColorTime, _lastSaveTime);
    drawGreenCrosshair(copyWithText);

    cv::imshow(_fractal.getFractalName(), copyWithText);

    _iterationN++;

    _isValid = true;
}

void FractalRenderer::setScreenSize(int width, int height)
{
    std::lock_guard<std::mutex> guard(_mutex);

    _screenWidth = width;
    _screenHeight = height;

    cv::resize(_renderedImage, _renderedImage, cv::Size(_screenWidth, _screenHeight));
}

uint64_t FractalRenderer::calculateFractalValues(cv::OutputArray fractalValues)
{
    const auto start = std::chrono::high_resolution_clock::now();

    cv::Mat resultMat = fractalValues.getMat();

    cv::parallel_for_(cv::Range(0, _screenWidth * _screenHeight), [&](const cv::Range& range) {
        for (int r = range.start; r < range.end; r++) {
            int x = r % _screenWidth;
            int y = r / _screenWidth;

            fractal_value_t n = _fractal.getFractalValue(x, _screenWidth, y, _screenHeight);
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

    mapGreyScaleImageToBGRA(pixelValues);

    if (_blendMode == NO_ALPHA) {
        renderedImage.assign(pixelValues);
    } else if (_blendMode == EPILEPSY) {
        cv::Mat renderedImageMat = renderedImage.getMat();
        pixelValues.forEach<uint8_color_t>([&](uint8_color_t &pixel, const int position[]) {
            uint8_color_t &oldPixel = renderedImageMat.at<uint8_color_t>(position[0], position[1]);
            oldPixel[0] += (uint8_t)(pixel[0] * _alpha);
            oldPixel[1] += (uint8_t)(pixel[1] * _alpha);
            oldPixel[2] += (uint8_t)(pixel[2] * _alpha);
            oldPixel[3] = pixel[3];
        });
    } else if (_blendMode == SMOOTH) {
        cv::addWeighted(renderedImage, 1.0 - _alpha, pixelValues, _alpha, 1, renderedImage);
    } else if (_blendMode == SATURATED) {
        renderedImage.getMat() += (pixelValues * _alpha);
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
    const float rec_totalHue = maxN / totalHue;
    for (int k = 0; k < maxN; ++k) {
        nToHue[k] *= rec_totalHue;
    }

    cv::parallel_for_(cv::Range(0, _screenWidth * _screenHeight), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            const unsigned int x = i % _screenWidth;
            const unsigned int y = i / _screenWidth;

            fractal_value_t &n = fractalValues.getMat().at<fractal_value_t>(y, x);
            n = nToHue[(uint16_t)n];
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

    if (_mkdir(_saveFolderName.c_str()) == -1) {
        if (errno != EEXIST) {
            std::string errorMessage("Folder \"" + _saveFolderName + "\" does not exist and cannot be created!");
            throw std::exception(errorMessage.c_str());
        }
    }

    std::string filePath = _saveFolderName + "/" + std::to_string(_iterationN) + ".png";

    cv::imwrite(filePath, image);

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

void FractalRenderer::drawInfoText(cv::InputOutputArray result, uint64_t calculateTime, uint64_t colorTime, uint64_t saveTime)
{
    std::stringstream ss(getInfoText());

    std::string line;
    int y = 25;
    /*   while (!std::getline(ss, line).eof()) {
           cv::putText(result, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, 0, 2);
           cv::putText(result, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, { 255, 255, 255 });
           y += 25;
       }
       */
}

void FractalRenderer::drawGreenCrosshair(cv::InputOutputArray result)
{
    const cv::Scalar colorGreen = cv::Scalar(50, 255, 50);

    cv::line(result,
        { _screenWidth / 2 - 10, _screenHeight / 2 - 10 },
        { _screenWidth / 2 + 10, _screenHeight / 2 + 10 },
        colorGreen, 1, cv::LineTypes::LINE_AA);

    cv::line(result,
        { _screenWidth / 2 - 10, _screenHeight / 2 + 10 },
        { _screenWidth / 2 + 10, _screenHeight / 2 - 10 },
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