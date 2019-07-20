#include <cmath>
#include "fractal.h"

#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286

Fractal::Fractal(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN) :
        _xCenter(xCenter),
        _yCenter(yCenter),
        _maxN(maxN),
        _zoom(zoom),
        _rotAngle(0)
{
}

coord_t Fractal::getFractalValue(int screenX, int screenWidth, int screenY, int screenHeight) const
{
    coord_t ratio = (coord_t) screenHeight / (coord_t) screenWidth;

    coord_t xMapped = interpolate(screenX, 0, screenWidth, (-1 / _zoom), (1 / _zoom));
    coord_t yMapped = interpolate(screenY, 0, screenHeight, (-1 / _zoom) * ratio, (1 / _zoom) * ratio);
    
    if (std::fabs(_rotAngle) > eps) {
        coord_t angleRad = (_rotAngle) * (M_PI / 180); // Convert to radians

        coord_t xRotated = cos(angleRad) * (xMapped) - sin(angleRad) * (yMapped);
        coord_t yRotated = sin(angleRad) * (xMapped) + cos(angleRad) * (yMapped);

        xMapped = xRotated;
        yMapped = yRotated;
    }

    xMapped += _xCenter;
    yMapped += _yCenter;

    return getFractalValue(xMapped, yMapped);
}

coord_t Fractal::interpolate(coord_t value, coord_t in_min, coord_t in_max, coord_t out_min, coord_t out_max)
{
    return (value - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

Vector2D_t Fractal::getDirectionVector(Direction direction, coord_t length, coord_t rotAngle)
{
    Vector2D_t result{0, 0};
    switch (direction) {
        case LEFT:
            result.x = -length;
            break;
        case RIGHT:
            result.x = length;
            break;
        case UP:
            result.y = -length;
            break;
        case DOWN:
            result.y = length;
            break;
        default:
            break;
    }

    if (fabs(rotAngle) > eps) {
        const coord_t angleRad = (rotAngle) * (M_PI / 180); // Convert to radians

        Vector2D_t rotatedVector = result;
        rotatedVector.x = cos(angleRad) * result.x - sin(angleRad) * result.y;
        rotatedVector.y = sin(angleRad) * result.x + cos(angleRad) * result.y;
        result = rotatedVector;
    }

    return result;
}

void Fractal::zoom(coord_t zoomFactor)
{
    //_maxN *= sqrt(_zoom_speed);
    _zoom *= zoomFactor;
}

void Fractal::move(Direction direction, coord_t relativeSpeed)
{
    coord_t absoluteSpeed = relativeSpeed / _zoom;

    Vector2D_t movementVector = getDirectionVector(direction, absoluteSpeed, _rotAngle);

    move(movementVector);
}

void Fractal::move(Vector2D_t movementVector)
{
    _xCenter += movementVector.x;
    _yCenter += movementVector.y;
}

void Fractal::rotate(coord_t deltaRot)
{
    _rotAngle += deltaRot;
}

coord_t Fractal::changeMaxN(coord_t delta)
{
    coord_t oldMaxN = _maxN;
    _maxN = std::fmaxf(_maxN + delta, 1);
    return _maxN - oldMaxN;
}

coord_t Fractal::getMaxN() const
{
    return _maxN;
}

coord_t Fractal::getZoom() const
{
    return _zoom;
}

coord_t Fractal::getXCenter() const
{
    return _xCenter;
}

coord_t Fractal::getYCenter() const
{
    return _yCenter;
}

coord_t Fractal::getRotAngle() const
{
    return _rotAngle;
}

void Fractal::setMaxN(coord_t maxN)
{
    _maxN = maxN;
}

void Fractal::setZoom(coord_t zoom)
{
    _zoom = zoom;
}

void Fractal::setXCenter(coord_t xCenter)
{
    _xCenter = xCenter;
}

void Fractal::setYCenter(coord_t yCenter)
{
    _yCenter = yCenter;
}

void Fractal::setRotAngle(coord_t rotAngle)
{
    _rotAngle = rotAngle;
}