#ifndef MANDELBROT_KEYBOARDCONTROLLER_H
#define MANDELBROT_KEYBOARDCONTROLLER_H


#include "controller.h"

class KeyboardController : public Controller
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
                       coord_t defaultMoveSpeed, coord_t defaultDeltaN, coord_t defaultDeltaRot,
                       float_color_t defaultDeltaColor);
    bool processKeyboardInput(int sym, bool turboMode);

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

private:
    coord_t _defaultZoomSpeed;
    coord_t _defaultMoveSpeed;
    coord_t _defaultDeltaN;
    coord_t _defaultDeltaRot;
    float_color_t _defaultDeltaColor;
};


#endif //MANDELBROT_KEYBOARDCONTROLLER_H
