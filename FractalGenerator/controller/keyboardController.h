#ifndef MANDELBROT_KEYBOARDCONTROLLER_H
#define MANDELBROT_KEYBOARDCONTROLLER_H

#include "UIController.h"
#include "keys.h"


class KeyboardController : public UIController
{
public:
    enum State
    {
        ZERO,
        PLUS,
        MINUS
    };

public:
    KeyboardController(Fractal &fractal, FractalRenderer &renderer, coord_t defaultZoomSpeed,
                       coord_t defaultMoveSpeed, fractal_value_t defaultDeltaN, coord_t defaultDeltaRot,
                       float_color_t defaultDeltaColor);

    bool processKeyboardInput(Keys key, bool shift);

    void setDefaultSpeeds(coord_t zoomSpeed, coord_t moveSpeed, fractal_value_t deltaResolution, coord_t deltaRot, float_color_t deltaColor);

private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void zoomIn();
    void zoomOut();

    void rotateLeft();
    void rotateRight();

    void increaseResolution();
    void decreaseResolution();

    void amazingResolution();
    void shittyResolution();

    void toggleZoomState(State zoomState);
    void toggleNState(State nState);
    void toggleRotState(State rotState);
    void toggleChangeColorState();

    coord_t _defaultZoomSpeed;
    coord_t _defaultMoveSpeed;
    fractal_value_t _defaultDeltaN;
    coord_t _defaultDeltaRot;
    float_color_t _defaultDeltaColor;
};


#endif //MANDELBROT_KEYBOARDCONTROLLER_H
