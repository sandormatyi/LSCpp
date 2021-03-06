#ifndef MANDELBROT_CONTROLLER_H
#define MANDELBROT_CONTROLLER_H

#include "../colors.h"

class Fractal;
class FractalRenderer;

class Controller
{
public:
    Controller(Fractal &fractal, FractalRenderer &renderer);

    Fractal & getFractal();
    FractalRenderer &getFractalRenderer();

    coord_t getZoomSpeed() const;
    coord_t getDeltaN() const;
    coord_t getDeltaRot() const;
    float_color_t getDeltaColor() const;

    void setZoomSpeed(coord_t zoomSpeed);
    void setDeltaN(coord_t deltaN);
    void setDeltaRot(coord_t deltaRot);
    void setDeltaColor(float_color_t color);

    bool doAutomaticTransformations();

    static bool isQuitFlagSet();
    static void setQuitFlag();

protected:
    Fractal &_fractal;
    FractalRenderer &_renderer;
    static bool _quit;

    coord_t _zoomSpeed = 1;
    coord_t _deltaN = 0;
    coord_t _deltaRot = 0;
    float_color_t _deltaColor {0,0,0,0};
};


#endif //MANDELBROT_CONTROLLER_H
