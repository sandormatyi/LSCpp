#include <cmath>
#include <iostream>
#include <limits>
#include "mandelbrot.h"


Mandelbrot::Mandelbrot(coord_t xCenter, coord_t yCenter, coord_t zoom, fractal_value_t maxN) :
        Fractal(xCenter, yCenter, zoom, maxN)
{
}


fractal_value_t Mandelbrot::getFractalValue(coord_t x, coord_t y) const
{
    coord_t x_init = x;
    coord_t y_init = y;

    // Cardioid / bulb checking
    coord_t q = ((x - 0.25) * (x - 0.25)) + (y * y);
    if ((q * (q + (x - 0.25))) <= 0.25 * y * y) {
        return 0;
    }

    for (int n = 0; n < _maxN; ++n) {
        coord_t x2 = x * x;
        coord_t y2 = y * y;

        if (x2 + y2 >= 4) {
            coord_t modulus = sqrt(x2 + y2);
            coord_t mu = n + 1 - (log(log2(modulus)));
            return mu;
        }

        coord_t x_next = x2 - y2 + x_init;
        coord_t y_next = 2 * x * y + y_init;

        if ((x == x_next && y == y_next)) {
            n = (int) _maxN + 1;
        } else {
            x = x_next;
            y = y_next;
        }
    }

    return 0;
}

const char *Mandelbrot::getFractalName()
{
    return "Mandelbrot";
}




