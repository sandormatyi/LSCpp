#include <cmath>
#include "keyboardController.h"
#include "../colors.h"


KeyboardController::KeyboardController(Fractal &fractal, FractalRenderer &renderer, coord_t defaultZoomSpeed,
                                       coord_t defaultMoveSpeed, coord_t defaultDeltaN, coord_t defaultDeltaRot,
                                       float_color_t defaultDeltaColor) :
        Controller(fractal, renderer),
        _defaultZoomSpeed(defaultZoomSpeed),
        _defaultMoveSpeed(defaultMoveSpeed),
        _defaultDeltaN(defaultDeltaN),
        _defaultDeltaRot(defaultDeltaRot),
        _defaultDeltaColor(defaultDeltaColor)
{
}

bool KeyboardController::processKeyboardInput(SDL_Keycode sym, bool turboMode)
{
    const coord_t multiplier = 10;

    coord_t zoomSpeed = _defaultZoomSpeed;
    coord_t moveSpeed = _defaultMoveSpeed;
    coord_t deltaRot = _defaultDeltaRot;
    coord_t deltaN = _defaultDeltaN;

    if (turboMode) {
        _defaultZoomSpeed *= multiplier;
        _defaultMoveSpeed *= multiplier;
        _defaultDeltaRot *= multiplier;
        _defaultDeltaN *= multiplier;
    }

    bool actionHappened = true;

    switch (sym) {
        case SDLK_w:
            zoomIn();
            break;
        case SDLK_s:
            zoomOut();
            break;
        case SDLK_UP:
            moveUp();
            break;
        case SDLK_DOWN:
            moveDown();
            break;
        case SDLK_LEFT:
            moveLeft();
            break;
        case SDLK_RIGHT:
            moveRight();
            break;
        case SDLK_q:
            decreaseResolution();
            break;
        case SDLK_e:
            increaseResolution();
            break;
        case SDLK_a:
            amazingResolution();
            break;
        case SDLK_d:
            shittyResolution();
            break;
        case SDLK_i:
            toggleZoomState(PLUS);
            break;
        case SDLK_o:
            toggleZoomState(MINUS);
            break;
        case SDLK_k:
            toggleNState(PLUS);
            break;
        case SDLK_l:
            toggleNState(MINUS);
            break;
        case SDLK_n:
            toggleRotState(PLUS);
            break;
        case SDLK_m:
            toggleRotState(MINUS);
            break;
        case SDLK_x:
            rotateLeft();
            break;
        case SDLK_c:
            rotateRight();
            break;
        case SDLK_p:
            toggleChangeColorState();
            break;
        default:
            actionHappened = false;
    }

    if (turboMode) {
        _defaultZoomSpeed = zoomSpeed;
        _defaultMoveSpeed = moveSpeed;
        _defaultDeltaRot = deltaRot;
        _defaultDeltaN = deltaN;
    }

    return actionHappened;
}

void KeyboardController::moveLeft()
{
    _fractal.move(LEFT, _defaultMoveSpeed);
}

void KeyboardController::moveRight()
{
    _fractal.move(RIGHT, _defaultMoveSpeed);
}

void KeyboardController::moveUp()
{
    _fractal.move(UP, _defaultMoveSpeed);
}

void KeyboardController::moveDown()
{
    _fractal.move(DOWN, _defaultMoveSpeed);
}

void KeyboardController::zoomIn()
{
    _fractal.zoom(_defaultZoomSpeed);
}

void KeyboardController::zoomOut()
{
    _fractal.zoom(1 / _defaultZoomSpeed);
}

void KeyboardController::increaseResolution()
{
    _fractal.changeMaxN(_defaultDeltaN);
}

void KeyboardController::decreaseResolution()
{
    _fractal.changeMaxN(-_defaultDeltaN);
}

void KeyboardController::amazingResolution()
{
    _fractal.setMaxN(1024);
}

void KeyboardController::shittyResolution()
{
    _fractal.setMaxN(8);
}

void KeyboardController::rotateLeft()
{
    _fractal.rotate(_defaultDeltaRot);
}

void KeyboardController::rotateRight()
{
    _fractal.rotate(- _defaultDeltaRot);
}

void KeyboardController::toggleZoomState(State zoomState)
{
    switch (zoomState) {
        case PLUS:
            if (_zoomSpeed > 1) {
                _zoomSpeed = 1;
            } else {
                _zoomSpeed = _defaultZoomSpeed;
            }
            break;
        case MINUS:
            if (_zoomSpeed < 1) {
                _zoomSpeed = 1;
            } else {
                _zoomSpeed = 1 / _defaultZoomSpeed;
            }
            break;
        case ZERO:
            _zoomSpeed = 1;
            break;
    }
}

void KeyboardController::toggleNState(State nState)
{
    switch (nState) {
        case PLUS:
            if (_deltaN > 0) {
                _deltaN = 0;
            } else {
                _deltaN = _defaultDeltaN;
            }
            break;
        case MINUS:
            if (_deltaN < 0) {
                _deltaN = 0;
            } else {
                _deltaN = -_defaultDeltaN;
            }
            break;
        case ZERO:
            _deltaN = 0;
            break;
    }
}

void KeyboardController::toggleRotState(State rotState)
{
    switch (rotState) {
        case PLUS:
            if (_deltaRot > 0) {
                _deltaRot = 0;
            } else {
                _deltaRot = _defaultDeltaRot;
            }
            break;
        case MINUS:
            if (_deltaRot < 0) {
                _deltaRot = 0;
            } else {
                _deltaRot = -_defaultDeltaRot;
            }
            break;
        case ZERO:
            _deltaRot = 0;
            break;
    }
}

void KeyboardController::toggleChangeColorState()
{
    if (isEmptyColor(_deltaColor)) {
        _deltaColor = _defaultDeltaColor;
    } else {
        _deltaColor = {0,0,0,0};
    }
}
