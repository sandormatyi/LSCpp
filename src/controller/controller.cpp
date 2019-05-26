#include <limits>
#include <cmath>
#include "controller.h"
#include "../colors.h"

Controller::Controller(Fractal &fractal, FractalRenderer &renderer) :
        _fractal(fractal), _renderer(renderer)
{

}

Fractal & Controller::getFractal()
{
    return _fractal;
}

FractalRenderer &Controller::getFractalRenderer()
{
    return _renderer;
}

coord_t Controller::getZoomSpeed() const
{
    return _zoomSpeed;
}

void Controller::setZoomSpeed(coord_t zoomSpeed)
{
    _zoomSpeed = zoomSpeed;
}

coord_t Controller::getDeltaN() const
{
    return _deltaN;
}

void Controller::setDeltaN(coord_t deltaN)
{
    _deltaN = deltaN;
}

coord_t Controller::getDeltaRot() const
{
    return _deltaRot;
}

void Controller::setDeltaRot(coord_t deltaRot)
{
    _deltaRot = deltaRot;
}

bool Controller::doAutomaticTransformations()
{
    bool invalidateImage = false;

    if ((_zoomSpeed > 1.0 + eps) || (_zoomSpeed < 1.0 - eps)) {
        _fractal.zoom(_zoomSpeed);
        invalidateImage = true;
    }

    if (fabs(_deltaRot) > eps) {
        _fractal.rotate(_deltaRot);
        invalidateImage = true;
    }

    if (fabs(_deltaN) > eps) {
        _fractal.changeMaxN(_deltaN);
        invalidateImage = true;
    }

    if (!isEmptyColor(_deltaColor)) {
        changeColors(_deltaColor.r, _deltaColor.g, _deltaColor.b, _deltaColor.a);
        invalidateImage = true;
    }

    return invalidateImage;
}

float_color_t Controller::getDeltaColor() const
{
    return _deltaColor;
}

void Controller::setDeltaColor(float_color_t color)
{
    _deltaColor = color;
}

bool Controller::isQuitFlagSet() const
{
    return _quit;
}

void Controller::setQuitFlag()
{
    _quit = true;
}
