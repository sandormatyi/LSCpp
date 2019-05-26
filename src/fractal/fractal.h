#ifndef MANDELBROT_FRACTAL_H
#define MANDELBROT_FRACTAL_H

#include <limits>

typedef double coord_t;

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Vector2D_t {
    coord_t x;
    coord_t y;
};

const coord_t eps = std::numeric_limits<coord_t>::epsilon() * 6;


class Fractal
{
public:
    Fractal(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN);

    coord_t getFractalValue(int screenX, int screenWidth, int screenY, int screenHeight) const;

    void zoom(coord_t zoomFactor);
    void move(Direction direction, coord_t relativeSpeed);
    void move(Vector2D_t vector);
    void rotate(coord_t deltaRot);
    coord_t changeMaxN(coord_t delta);

    coord_t getXCenter() const;
    coord_t getYCenter() const;
    coord_t getMaxN() const;
    coord_t getZoom() const;
    coord_t getRotAngle() const;

    void setMaxN(coord_t maxN);
    void setZoom(coord_t zoom);
    void setXCenter(coord_t xCenter);
    void setYCenter(coord_t yCenter);
    void setRotAngle(coord_t rotAngle);

    virtual const char* getFractalName() = 0;

protected:
    virtual coord_t getFractalValue(coord_t mappedX, coord_t mappedY) const = 0;

    static coord_t interpolate(coord_t value, coord_t in_min, coord_t in_max, coord_t out_min, coord_t out_max);
    static Vector2D_t getDirectionVector(Direction direction, coord_t length, coord_t rotAngle);

    coord_t _xCenter;
    coord_t _yCenter;
    coord_t _maxN;
    coord_t _zoom;
    coord_t _rotAngle;
};


#endif //MANDELBROT_FRACTAL_H
