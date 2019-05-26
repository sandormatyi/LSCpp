#ifndef MANDELBROT_FRACTALRENDERER_H
#define MANDELBROT_FRACTALRENDERER_H

#include <string>
#include <SDL_types.h>
#include <SDL_render.h>
#include <SDL_atomic.h>
#include <ctime>
#include "../fractal/fractal.h"
#include "renderParams.h"

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

struct thread_data
{
    static const int max = TEXTURE_WIDTH * TEXTURE_HEIGHT / THREAD_NUMBER;
    int thread_number;
    coord_t n[max];
    const Fractal *fractal;
    SDL_atomic_t *counter;
};


class FractalRenderer
{
public:
    FractalRenderer(Fractal &fractal, SDL_Renderer *renderer, const std::string &folderName);
    ~FractalRenderer();

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
    Uint64 calculateFractalValues();
    Uint64 colorPixels();
    Uint64 saveImage(SDL_Texture *texture);

    void drawInfoText(Uint64 calculateTime, Uint64 colorTime, Uint64 saveTime);
    void drawGreenCrosshair();

    void colorByHistogram();
    void colorLinear();
    void colorPixel(const SDL_Color &c, int offset);

private:
    Fractal &_fractal;
    SDL_Renderer *_renderer;

    bool _isValid = false;
    bool _enableRender = true;
    bool _saveImage = false;
    const std::string _folderName;

    coord_t _fadeFactor = 0.75;
    BlendMode _blendMode = NO_ALPHA;
    ColorMode _colorMode = LINEAR;
    TraceMode _traceMode = DISABLE;

    int _iterationN = 1;

    Uint8 *_pixels;
    thread_data *_data;
};


#endif //MANDELBROT_FRACTALRENDERER_H
