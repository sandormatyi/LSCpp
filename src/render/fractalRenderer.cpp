#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <atomic>
#include "fractalRenderer.h"
#include "../colors.h"
#include "renderedText.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <chrono>  // for high_resolution_clock
#include <direct.h> // _mkdir
#include <thread>
#include <map>


FractalRenderer::FractalRenderer(Fractal &fractal, cv::InputOutputArray matrix, const std::string &folderName) :
    _matrix(matrix),
    _fractal(fractal),
    _folderName(std::string("C:/Users/matyi/Pictures/fractal/") + folderName + "_" + std::to_string(time(nullptr))),
    _histogram(true),
    _blendMode(NO_ALPHA),
    _traceMode(DISABLE)
{
}

void FractalRenderer::invalidate()
{
    _isValid = false;
}

void FractalRenderer::setBlendMode(BlendMode blendMode)
{
    _blendMode = blendMode;
}

void FractalRenderer::setTraceMode(TraceMode traceMode)
{
    _traceMode = traceMode;
}

void FractalRenderer::setSaveImage(bool saveImage)
{
    _saveImage = saveImage;
}

void FractalRenderer::render()
{
    if (_isValid)
        return;

    uint64_t calculateTime = 0;
    uint64_t colorTime = 0;
    uint64_t saveTime = 0;

    if (_enableRender) {
        cv::Mat fractalValues(SCREEN_HEIGHT, SCREEN_WIDTH, CV_32FC1);

        calculateTime = calculateFractalValues(fractalValues);

        colorTime = colorPixels(fractalValues);

        morphImage(_matrix);

        if (_saveImage) {
            saveTime = saveImage(_matrix);
        }
    }

    cv::Mat copyWithText;
    _matrix.copyTo(copyWithText);

    drawInfoText(copyWithText, calculateTime, colorTime, saveTime);
    drawGreenCrosshair(copyWithText);

    cv::imshow(_fractal.getFractalName(), copyWithText);

    _iterationN++;

    _isValid = true;
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
    nToHue[0] = 0;
    fractal_value_t totalHue = 0;
    for (int k = 1; k < maxN; ++k) {
        totalHue += hist.at<uint16_t>(k);
        nToHue[k] = totalHue;
    }
    const double rec_totalHue = maxN / totalHue;
    for (int k = 1; k < maxN; ++k) {
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

    textToTexture(ss.str(), result);
}

uint64_t FractalRenderer::calculateFractalValues(cv::OutputArray result)
{
    const auto start = std::chrono::high_resolution_clock::now();

    cv::Mat &resultMat = result.getMatRef();

    cv::parallel_for_(cv::Range(0, SCREEN_WIDTH * SCREEN_HEIGHT), [&](const cv::Range& range) {
        for (int r = range.start; r < range.end; r++) {
            int x = r % _matrix.cols();
            int y = r / _matrix.cols();

            fractal_value_t n = _fractal.getFractalValue(x, SCREEN_WIDTH, y, SCREEN_HEIGHT);
            resultMat.at<fractal_value_t>(y, x) = n;
        }
    });

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

uint64_t FractalRenderer::colorPixels(cv::InputOutputArray fractalValues)
{
    const auto start = std::chrono::high_resolution_clock::now();

    if (_traceMode == DISABLE) {
        _matrix.getMat() = cv::Scalar{ 0,0,0,0 };
    } else if (_traceMode == FADE_ALL) {
        _matrix.getMat() *= _fadeFactor;
    }

    if (_histogram) {
        equalizeByHistogram(fractalValues);
    }

    colorLinear(fractalValues);

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

uint64_t FractalRenderer::morphImage(cv::InputOutputArray image)
{
    const auto start = std::chrono::high_resolution_clock::now();

    // TODO

    const auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return (uint64_t)(diff.count() * 1000);
}

void FractalRenderer::colorLinear(cv::InputArray fractalValues)
{
    const float maxN = _fractal.getMaxN();

    cv::parallel_for_(cv::Range(0, SCREEN_WIDTH*SCREEN_HEIGHT), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            const unsigned int x = i % TEXTURE_WIDTH;
            const unsigned int y = i / TEXTURE_WIDTH;

            fractal_value_t n = fractalValues.getMat().at<fractal_value_t>(y, x);

            if (n > 0) {
                fractal_value_t f = n / maxN;
                int colorIndex = getColorIndex(f);
                float_color_t &c1 = getColor(colorIndex);
                float_color_t &c2 = getColor(colorIndex + 1);
                float_color_t c = interpolate(c1, c2, f - (int)(f));

                colorPixel(to_SDL_Color(c), x, y);
            }
        }
    });
}

void FractalRenderer::colorPixel(const uint8_color_t &c, int x, int y)
{
    float floatAlpha = c.a / 255.0;
    float oldAlpha = 1 - floatAlpha;

    cv::Vec4b &p = _matrix.getMat().at<cv::Vec4b>(y, x);

    // TODO: Remove FADE_FILLED
    if (_traceMode == FADE_FILLED) {
        p *= _fadeFactor;
    }

    switch (_blendMode) {
        case NO_ALPHA:
            p[0] = c.b;
            p[1] = c.g;
            p[2] = c.r;
            p[3] = c.a;
            break;
        case EPILEPSY:
            p[0] += (uint8_t)(floatAlpha * c.b);
            p[1] += (uint8_t)(floatAlpha * c.g);
            p[2] += (uint8_t)(floatAlpha * c.r);
            p[3] = (uint8_t)(floatAlpha * c.a);
            break;
        case SMOOTH:
            p[0] = (uint8_t)((oldAlpha * p[0]) + (floatAlpha * c.b));
            p[1] = (uint8_t)((oldAlpha * p[1]) + (floatAlpha * c.g));
            p[2] = (uint8_t)((oldAlpha * p[2]) + (floatAlpha * c.r));
            p[3] = (uint8_t)(floatAlpha * c.a);
            break;
        default:
            break;
    }
}

bool FractalRenderer::getSaveImage() const
{
    return _saveImage;
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

void FractalRenderer::setEnableRender(bool enable)
{
    _enableRender = enable;
}

void FractalRenderer::setFadeFactor(coord_t fadeFactor)
{
    _fadeFactor = fadeFactor;
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