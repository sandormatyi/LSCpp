#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "fractal.h"

class Mandelbrot : public Fractal
{
public:
    Mandelbrot(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN);
    const char *getFractalName() override;

protected:
    coord_t getFractalValue(coord_t x, coord_t y) const override;
};


#endif //MANDELBROT_MANDELBROT_H
