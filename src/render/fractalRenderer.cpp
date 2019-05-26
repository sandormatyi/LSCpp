#include <SDL_timer.h>
#include <string>
#include <cstring>
#include <SDL_thread.h>
#include <sstream>
#include <io.h>
#include <SDL_image.h>
#include <cmath>
#include <iomanip>
#include "fractalRenderer.h"
#include "../colors.h"
#include "renderedText.h"

static int drawPointsInThread(void *data)
{
    thread_data *threadData = (thread_data *) data;

    for (int i = 0; i < thread_data::max; ++i) {
        int x = (i % TEXTURE_WIDTH);
        int y = i / TEXTURE_WIDTH + TEXTURE_HEIGHT * threadData->thread_number / THREAD_NUMBER;
        threadData->n[i] = threadData->fractal->getFractalValue(x, TEXTURE_WIDTH, y, TEXTURE_HEIGHT);
    }

    SDL_AtomicDecRef(threadData->counter);

    return 0;
}

FractalRenderer::FractalRenderer(Fractal &fractal, SDL_Renderer *renderer, const std::string &folderName) :
        _fractal(fractal),
        _renderer(renderer),
        _folderName(std::string("C:/Users/matyi/Pictures/fractal/") + folderName + "_" + std::to_string(time(nullptr)))
{
    _pixels = (Uint8 *) malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT * 4);
    _data = (thread_data *) malloc(THREAD_NUMBER * sizeof(thread_data));
}

FractalRenderer::~FractalRenderer()
{
    delete (_pixels);
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
    static SDL_Texture *texture = SDL_CreateTexture(
            _renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            TEXTURE_WIDTH, TEXTURE_HEIGHT
    );

    if (_isValid)
        return;

    //Clear screen
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_renderer);

    Uint64 calculateTime = 0;
    Uint64 colorTime = 0;
    Uint64 saveTime = 0;

    if (_enableRender) {
        calculateTime = calculateFractalValues();

        colorTime = colorPixels();

        SDL_UpdateTexture(
                texture,
                nullptr,
                _pixels,
                TEXTURE_WIDTH * 4
        );

        SDL_RenderCopy(_renderer, texture, nullptr, nullptr);

        if (_saveImage) {
            saveTime = saveImage(texture);
        }
    }

    drawInfoText(calculateTime, colorTime, saveTime);
    drawGreenCrosshair();

    //Update screen
    SDL_RenderPresent(_renderer);

    _iterationN++;

    _isValid = true;
}

void FractalRenderer::drawInfoText(Uint64 calculateTime, Uint64 colorTime, Uint64 saveTime)
{
    // Draw text info
    static SDL_Texture *texture = nullptr;

    SDL_DestroyTexture(texture);

    const static Uint64 freq = SDL_GetPerformanceFrequency();
    const Uint64 calculateMs = calculateTime * 1000 / freq;
    const Uint64 colorMs = colorTime * 1000 / freq;
    const Uint64 saveMs = saveTime * 1000 / freq;

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

    RenderedText text = textToTexture(ss.str(), _renderer, 600);
    texture = text._texture;

    SDL_Rect rect = {0, 0, text._width, text._height};
    SDL_RenderCopyEx(_renderer, text._texture, nullptr, &rect, 0.0, nullptr, SDL_FLIP_NONE);
}

Uint64 FractalRenderer::calculateFractalValues()
{
    SDL_atomic_t counter{THREAD_NUMBER};

    const Uint64 start = SDL_GetPerformanceCounter();

    for (int i = 0; i < THREAD_NUMBER; ++i) {
        _data[i].counter = &counter;
        _data[i].fractal = &_fractal;
        _data[i].thread_number = i;
        char thread_name[10];
        strcpy(thread_name, ("Thread " + std::to_string(i)).c_str());

        SDL_CreateThread(drawPointsInThread, thread_name, &_data[i]);
    }

    while (SDL_AtomicGet(&counter) > 0);

    const Uint64 end = SDL_GetPerformanceCounter();
    return end - start;
}

Uint64 FractalRenderer::colorPixels()
{
    const Uint64 start = SDL_GetPerformanceCounter();

    if (_traceMode == DISABLE) {
        memset(_pixels, 0, TEXTURE_WIDTH * TEXTURE_HEIGHT * 4);
    } else if (_traceMode == FADE_FILLED) {

    }

    switch (_colorMode) {
        case HISTOGRAM:
            colorByHistogram();
            break;
        case LINEAR:
            colorLinear();
            break;
    }

    const Uint64 end = SDL_GetPerformanceCounter();
    return end - start;
}

void FractalRenderer::colorByHistogram()
{
    const int maxN = (int) (_fractal.getMaxN() + 1);

    int histogram[maxN];
    memset(histogram, 0, maxN * sizeof(int));

    for (int i = 0; i < THREAD_NUMBER; ++i) {
        const thread_data *threadData = &_data[i];

        for (int j = 0; j < thread_data::max; ++j) {
            histogram[(int) threadData->n[j]]++;
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
            const unsigned int offset = 4 * (threadData->thread_number * thread_data::max + j);

            if (_traceMode == FADE_ALL) {
                _pixels[offset + 0] *= _fadeFactor;
                _pixels[offset + 1] *= _fadeFactor;
                _pixels[offset + 2] *= _fadeFactor;
            }

            if (n > 0) {
                coord_t hue = 0.0;
                for (int k = 0; k <= n; ++k) {
                    hue += histogram[k] / total;
                }

                int colorIndex = getColorIndex(hue);
                float_color_t &c = getColor(colorIndex);

                colorPixel(to_SDL_Color(c), offset);
            }
        }
    }
}

void FractalRenderer::colorLinear()
{
    const coord_t maxN = _fractal.getMaxN();

    for (int i = 0; i < THREAD_NUMBER; ++i) {
        const thread_data *threadData = &_data[i];

        for (int j = 0; j < thread_data::max; ++j) {
            const unsigned int x = j % TEXTURE_WIDTH;
            const unsigned int y = j / TEXTURE_WIDTH + threadData->thread_number * TEXTURE_HEIGHT / THREAD_NUMBER;
            const unsigned int offset = (TEXTURE_WIDTH * 4 * y) + x * 4;

            if (_traceMode == FADE_ALL) {
                _pixels[offset + 0] *= _fadeFactor;
                _pixels[offset + 1] *= _fadeFactor;
                _pixels[offset + 2] *= _fadeFactor;
            }

            coord_t n = threadData->n[j];

            if (n > 0) {
                coord_t f = n / maxN;
                int colorIndex = getColorIndex(f);
                float_color_t &c1 = getColor(colorIndex);
                float_color_t &c2 = getColor(colorIndex + 1);
                float_color_t c = interpolate(c1, c2, f - (int) (f));

                colorPixel(to_SDL_Color(c), offset);
            }
        }
    }
}

void FractalRenderer::colorPixel(const SDL_Color &c, int offset)
{
    float floatAlpha = c.a / 255.0;
    float oldAlpha = 1 - floatAlpha;

    if (_traceMode == FADE_FILLED) {
        _pixels[offset + 0] *= _fadeFactor;
        _pixels[offset + 1] *= _fadeFactor;
        _pixels[offset + 2] *= _fadeFactor;
    }

    switch (_blendMode) {
        case NO_ALPHA:
            _pixels[offset + 0] = c.b;
            _pixels[offset + 1] = c.g;
            _pixels[offset + 2] = c.r;
            _pixels[offset + 3] = c.a;
            break;
        case EPILEPSY:
            _pixels[offset + 0] += (Uint8) (floatAlpha * c.b);
            _pixels[offset + 1] += (Uint8) (floatAlpha * c.g);
            _pixels[offset + 2] += (Uint8) (floatAlpha * c.r);
            _pixels[offset + 3] = (Uint8) (floatAlpha * c.a);
            break;
        case SMOOTH:
            _pixels[offset + 0] = (Uint8) ((oldAlpha * _pixels[offset + 0]) + (floatAlpha * c.b));
            _pixels[offset + 1] = (Uint8) ((oldAlpha * _pixels[offset + 1]) + (floatAlpha * c.g));
            _pixels[offset + 2] = (Uint8) ((oldAlpha * _pixels[offset + 2]) + (floatAlpha * c.r));
            _pixels[offset + 3] = (Uint8) (floatAlpha * c.a);
            break;
        default:
            break;
    }
}

bool FractalRenderer::getSaveImage() const
{
    return _saveImage;
}

Uint64 FractalRenderer::saveImage(SDL_Texture *texture)
{
    const Uint64 start = SDL_GetPerformanceCounter();

    static bool folderExists = false;
    if (!folderExists) {
        mkdir(_folderName.c_str());
        folderExists = true;
    }

    std::string filePath = _folderName + "/" + std::to_string(_iterationN) + ".png";

    SDL_Texture *target = SDL_GetRenderTarget(_renderer);
    SDL_SetRenderTarget(_renderer, texture);
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(_renderer, nullptr, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, filePath.c_str());
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(_renderer, target);

    const Uint64 end = SDL_GetPerformanceCounter();
    return end - start;
}

void FractalRenderer::drawGreenCrosshair()
{
    SDL_SetRenderDrawColor(_renderer, 50, 255, 50, 0xFF);
    SDL_RenderDrawLine(_renderer, TEXTURE_WIDTH / 2 - 10, TEXTURE_HEIGHT / 2 - 10, TEXTURE_WIDTH / 2 + 10,
                       TEXTURE_HEIGHT / 2 + 10);
    SDL_RenderDrawLine(_renderer, TEXTURE_WIDTH / 2 - 10, TEXTURE_HEIGHT / 2 + 10, TEXTURE_WIDTH / 2 + 10,
                       TEXTURE_HEIGHT / 2 - 10);
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

