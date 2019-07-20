#ifndef MANDELBROT_FRACTAL_H
#define MANDELBROT_FRACTAL_H

#include <limits>
#include "../types.h"

const coord_t eps = std::numeric_limits<float>::epsilon() * 6;


class Fractal
{
public:
    Fractal(coord_t xCenter, coord_t yCenter, coord_t zoom, fractal_value_t maxN);

    fractal_value_t getFractalValue(int screenX, int screenWidth, int screenY, int screenHeight) const;

    void zoom(coord_t zoomFactor);
    void move(Direction direction, coord_t relativeSpeed);
    void move(Vector2D_t vector);
    void rotate(coord_t deltaRot);
    fractal_value_t changeMaxN(fractal_value_t delta);

    coord_t getXCenter() const;
    coord_t getYCenter() const;
    fractal_value_t getMaxN() const;
    coord_t getZoom() const;
    coord_t getRotAngle() const;

    void setMaxN(fractal_value_t maxN);
    void setZoom(coord_t zoom);
    void setXCenter(coord_t xCenter);
    void setYCenter(coord_t yCenter);
    void setRotAngle(coord_t rotAngle);

    virtual const char* getFractalName() = 0;

protected:
    virtual fractal_value_t getFractalValue(coord_t mappedX, coord_t mappedY) const = 0;

    static coord_t interpolate(coord_t value, coord_t in_min, coord_t in_max, coord_t out_min, coord_t out_max);
    static Vector2D_t getDirectionVector(Direction direction, coord_t length, coord_t rotAngle);

    coord_t _xCenter;
    coord_t _yCenter;
    fractal_value_t _maxN;
    coord_t _zoom;
    coord_t _rotAngle;
};


#endif //MANDELBROT_FRACTAL_H
