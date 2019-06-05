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


static void drawPointsInThread(thread_data *threadData)
{
    for (int i = 0; i < thread_data::max; ++i) {
        int x = (i % TEXTURE_WIDTH);
        int y = i / TEXTURE_WIDTH + TEXTURE_HEIGHT * threadData->thread_number / THREAD_NUMBER;
        threadData->n[i] = threadData->fractal->getFractalValue(x, TEXTURE_WIDTH, y, TEXTURE_HEIGHT);
    }
    (*(threadData->counter))--;
}

FractalRenderer::FractalRenderer(Fractal &fractal, cv::InputOutputArray matrix, const std::string &folderName) :
    _matrix(matrix),
    _fractal(fractal),
    _folderName(std::string("C:/Users/matyi/Pictures/fractal/") + folderName + "_" + std::to_string(time(nullptr)))
{
    _data = (thread_data *) malloc(THREAD_NUMBER * sizeof(thread_data));
}

FractalRenderer::~FractalRenderer()
{
    delete (_data);
}

void FractalRenderer::invalidate()
{
    _isValid = false;
}

void FractalRenderer::setBlendMode(BlendMode blendMode)
{
    _blendMode = blendMode;
}

void FractalRenderer::setColorMode(ColorMode colorMode)
{
    _colorMode = colorMode;
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

    //Clear screen
    //SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0x00);
    //SDL_RenderClear(_renderer);

    uint64_t calculateTime = 0;
    uint64_t colorTime = 0;
    uint64_t saveTime = 0;

    if (_enableRender) {
        cv::Mat fractalValues(SCREEN_HEIGHT, SCREEN_WIDTH, CV_64FC1);

        calculateTime = calculateFractalValues(fractalValues);

        colorTime = colorPixels(fractalValues);

        if (_saveImage) {
            saveTime = saveImage();
        }
    }

    drawInfoText(calculateTime, colorTime, saveTime);
    drawGreenCrosshair();

    cv::imshow("Main window", _matrix);

    _iterationN++;

    _isValid = true;
}

void FractalRenderer::drawInfoText(uint64_t calculateTime, uint64_t colorTime, uint64_t saveTime)
{
    const static uint64_t freq = 1;
    const uint64_t calculateMs = calculateTime * 1000 / freq;
    const uint64_t colorMs = colorTime * 1000 / freq;
    const uint64_t saveMs = saveTime * 1000 / freq;

    std::stringstream ss;
    // ss << _fractal.getFractalName() << std::endl;

    ss << "X: " << std::setprecision(15) << _fractal.getXCenter() << std::endl;
    ss << "Y: " << std::setprecision(15) << _fractal.getYCenter() << std::endl;
    ss << "Zoom: " << _fractal.getZoom() << std::endl;
    ss << "Resolution: " << _fractal.getMaxN() << std::endl;
    ss << "Rotation: " << _fractal.getRotAngle() << std::endl;
    ss << "Render time: " << calculateMs + colorMs + saveMs << " ms (" << calculateMs << " + " << colorMs << " + "
       << saveMs << ")" << std::endl;
    // ss << "Threads: " << THREAD_NUMBER << std::endl;
    ss << "Color mode: " << to_string(_colorMode) << std::endl;
    ss << "Blend mode: " << to_string(_blendMode) << std::endl;
    ss << "Trace mode: " << to_string(_traceMode) << std::endl;
    if (_saveImage) {
        ss << "Image number " << _iterationN - 1;
    }

    textToTexture(ss.str(), _matrix, 600);
}

uint64_t FractalRenderer::calculateFractalValues(cv::OutputArray result)
{
    const auto start = std::chrono::high_resolution_clock::now();
    std::atomic<uint32_t> counter = THREAD_NUMBER;

    cv::parallel_for_(cv::Range(0, SCREEN_WIDTH*SCREEN_HEIGHT), [&](const cv::Range& range) {
        for (int r = range.start; r < range.end; r++) {
            int x = r % _matrix.cols();
            int y = r / _matrix.cols();

            coord_t value = _fractal.getFractalValue(x, SCREEN_WIDTH, y, SCREEN_HEIGHT);
            result.getMat().at<coord_t>(y, x) = value;
        }
    });

//#pragma omp parallel for
//    for (int r = range.start; r < range.end; r++) {
//        int y = r / _matrix.cols();
//        int x = r % _matrix.cols();
//
//        coord_t value = _fractal.getFractalValue(x, SCREEN_WIDTH, y, SCREEN_HEIGHT);
//        result.getMat().at<coord_t>(y, x) = value;
//    }

    //for (int i = 0; i < THREAD_NUMBER; ++i) {
    //    _data[i].fractal = &_fractal;
    //    _data[i].thread_number = i;
    //    _data[i].counter = &counter;
    //    std::thread thread(drawPointsInThread, &_data[i]);
    //    thread.join(); // TODO: Ez gányolás
    //}

    //while (counter > 0);

    const auto end = std::chrono::high_resolution_clock::now();

    return (end - start).count();
}

uint64_t FractalRenderer::colorPixels(cv::InputArray fractalValues)
{
    const auto start = std::chrono::high_resolution_clock::now();

    if (_traceMode == DISABLE) {
        _matrix.getMat() = cv::Scalar{ 0,0,0,0 };
    } else if (_traceMode == FADE_ALL) {
        _matrix.getMat() *= _fadeFactor;
    }

    switch (_colorMode) {
        case HISTOGRAM:
            colorByHistogram(fractalValues);
            break;
        case LINEAR:
            colorLinear(fractalValues);
            break;
    }

    const auto end = std::chrono::high_resolution_clock::now();
    return (end - start).count();
}

void FractalRenderer::colorByHistogram(cv::InputArray fractalValues)
{
    const int maxN = (int) (_fractal.getMaxN() + 1);

    int* histogram = new int[maxN];
    memset(histogram, 0, maxN * sizeof(int));

    for (int i = 0; i < THREAD_NUMBER; ++i) {
        const thread_data *threadData = &_data[i];

        for (int j = 0; j < thread_data::max; ++j) {
            histogram[(int)fractalValues.getMat().at<coord_t>(j)]++;
        }
    }

    coord_t total = 0;
    for (int i = 0; i < maxN; ++i) {
        total += histogram[i];
    }

    for (int i = 0; i < THREAD_NUMBER; ++i) {
        const thread_data *threadData = &_data[i];

        for (int j = 0; j < thread_data::max; ++j) {
            coord_t n = threadData->n[j];
            const unsigned int x = j % TEXTURE_WIDTH;
            const unsigned int y = j / TEXTURE_WIDTH + threadData->thread_number * TEXTURE_HEIGHT / THREAD_NUMBER;

            if (n > 0) {
                coord_t hue = 0.0;
                for (int k = 0; k <= n; ++k) {
                    hue += histogram[k] / total;
                }

                int colorIndex = getColorIndex(hue);
                float_color_t &c = getColor(colorIndex);

                colorPixel(to_SDL_Color(c), x, y);
            }
        }
    }
    delete[] histogram;
}

void FractalRenderer::colorLinear(cv::InputArray fractalValues)
{
    const coord_t maxN = _fractal.getMaxN();

    cv::parallel_for_(cv::Range(0, SCREEN_WIDTH*SCREEN_HEIGHT), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            const unsigned int x = i % TEXTURE_WIDTH;
            const unsigned int y = i / TEXTURE_WIDTH;

            coord_t n = fractalValues.getMat().at<coord_t>(y, x);

            if (n > 0) {
                coord_t f = n / maxN;
                int colorIndex = getColorIndex(f);
                float_color_t &c1 = getColor(colorIndex);
                float_color_t &c2 = getColor(colorIndex + 1);
                float_color_t c = interpolate(c1, c2, f - (int)(f));

                colorPixel(to_SDL_Color(c), x, y);
            }
        }
    });
}

void FractalRenderer::colorPixel(const SDL_Color &c, int x, int y)
{
    float floatAlpha = c.a / 255.0;
    float oldAlpha = 1 - floatAlpha;

    cv::Vec4b &p = _matrix.getMat().at<cv::Vec4b>(y, x);

    // TODO: Remove FADE_FILLED
    if (_traceMode == FADE_FILLED) {
        p[0] *= _fadeFactor;
        p[1] *= _fadeFactor;
        p[2] *= _fadeFactor;
    }

    switch (_blendMode) {
        case NO_ALPHA:
            p[0] = c.b;
            p[1] = c.g;
            p[2] = c.r;
            p[3] = c.a;
            break;
        case EPILEPSY:
            p[0] += (uint8_t) (floatAlpha * c.b);
            p[1] += (uint8_t) (floatAlpha * c.g);
            p[2] += (uint8_t) (floatAlpha * c.r);
            p[3] = (uint8_t) (floatAlpha * c.a);
            break;
        case SMOOTH:
            p[0] = (uint8_t) ((oldAlpha * p[0]) + (floatAlpha * c.b));
            p[1] = (uint8_t) ((oldAlpha * p[1]) + (floatAlpha * c.g));
            p[2] = (uint8_t) ((oldAlpha * p[2]) + (floatAlpha * c.r));
            p[3] = (uint8_t) (floatAlpha * c.a);
            break;
        default:
            break;
    }
}

bool FractalRenderer::getSaveImage() const
{
    return _saveImage;
}

uint64_t FractalRenderer::saveImage()
{
    const auto start = std::chrono::high_resolution_clock::now();

    static bool folderExists = false;
    if (!folderExists) {
        _mkdir(_folderName.c_str());
        folderExists = true;
    }

    std::string filePath = _folderName + "/" + std::to_string(_iterationN) + ".png";

    cv::imwrite(filePath, _matrix);

    const auto end = std::chrono::high_resolution_clock::now();
    return (end - start).count();
}

void FractalRenderer::drawGreenCrosshair()
{
    const cv::Scalar colorGreen = cv::Scalar(50, 255, 50);

    cv::line(_matrix, 
        { TEXTURE_WIDTH / 2 - 10, TEXTURE_HEIGHT / 2 - 10 }, 
        { TEXTURE_WIDTH / 2 + 10, TEXTURE_HEIGHT / 2 + 10 }, 
        colorGreen);

    cv::line(_matrix,
        { TEXTURE_WIDTH / 2 - 10, TEXTURE_HEIGHT / 2 + 10 },
        { TEXTURE_WIDTH / 2 + 10, TEXTURE_HEIGHT / 2 - 10 },
        colorGreen);
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

std::string to_string(ColorMode colorMode)
{
    switch (colorMode) {
        case LINEAR:
            return "LINEAR";
        case HISTOGRAM:
            return "HISTOGRAM";
    }
    throw std::invalid_argument("colorMode");
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

