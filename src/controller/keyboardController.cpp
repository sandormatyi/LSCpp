#include <cmath>
#include "keyboardController.h"
#include "../colors.h"
#include "../fractal/fractal.h"
#include "../render/fractalRenderer.h"

const int KEY_ESCAPE = 27;
const int KEY_SPACE = 32;
const int KEY_LEFT_ARROW = 2424832;
const int KEY_RIGHT_ARROW = 2555904;
const int KEY_UP_ARROW = 2490368;
const int KEY_DOWN_ARROW = 2621440;
const int KEY_F1 = 7340032;
const int KEY_F2 = 7405568;
const int KEY_F3 = 7471104;
const int KEY_F4 = 7536640;


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

bool KeyboardController::processKeyboardInput(int sym)
{
    const coord_t multiplier = 10;

    coord_t zoomSpeed = _defaultZoomSpeed;
    coord_t moveSpeed = _defaultMoveSpeed;
    coord_t deltaRot = _defaultDeltaRot;
    coord_t deltaN = _defaultDeltaN;

    bool turboMode = sym > 64 && sym < 91;

    if (turboMode) {
        _defaultZoomSpeed *= multiplier;
        _defaultMoveSpeed *= multiplier;
        _defaultDeltaRot *= multiplier;
        _defaultDeltaN *= multiplier;
    }

    bool actionHappened = true;

    switch (sym) {
    case 'w':
    case 'W':
        zoomIn();
        break;
    case 's':
    case 'S':
        zoomOut();
        break;
    case KEY_UP_ARROW:
        moveUp();
        break;
    case KEY_DOWN_ARROW:
        moveDown();
        break;
    case KEY_LEFT_ARROW:
        moveLeft();
        break;
    case KEY_RIGHT_ARROW:
        moveRight();
        break;
    case 'q':
    case 'Q':
        decreaseResolution();
        break;
    case 'e':
    case 'E':
        increaseResolution();
        break;
    case 'a':
    case 'A':
        amazingResolution();
        break;
    case 'd':
    case 'D':
        shittyResolution();
        break;
    case 'i':
    case 'I':
        toggleZoomState(PLUS);
        break;
    case 'o':
    case 'O':
        toggleZoomState(MINUS);
        break;
    case 'k':
    case 'K':
        toggleNState(PLUS);
        break;
    case 'l':
    case 'L':
        toggleNState(MINUS);
        break;
    case 'n':
    case 'N':
        toggleRotState(PLUS);
        break;
    case 'm':
    case 'M':
        toggleRotState(MINUS);
        break;
    case 'x':
    case 'X':
        rotateLeft();
        break;
    case 'c':
    case 'C':
        rotateRight();
        break;
    case 'p':
    case 'P':
        toggleChangeColorState();
        break;
    case KEY_ESCAPE:
        setQuitFlag();
        break;
    case 'r':
    case 'R':
        _renderer.setSaveImage(!_renderer.getSaveImage());
        break;
    case '1':
        _renderer.setBlendMode(NO_ALPHA);
        break;
    case '2':
        _renderer.setBlendMode(SMOOTH);
        break;
    case '3':
        _renderer.setBlendMode(EPILEPSY);
        break;
    case '4':
        _renderer.setBlendMode(SATURATED);
        break;
    case KEY_F1:
        _renderer.setTraceMode(DISABLE);
        break;
    case KEY_F2:
        _renderer.setTraceMode(PERSIST);
        break;
    case KEY_F3:
        _renderer.setTraceMode(FADE_FILLED);
        break;
    case KEY_F4:
        _renderer.setTraceMode(FADE_ALL);
        break;
    /*case KEY_SPACE:
        scoreEnabled = !scoreEnabled;
        break;
    case SDLK_BACKSPACE:
        automaticController.undoLastBeat();
        fractalRenderer.invalidate();
        break;*/
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
    _fractal.rotate(-_defaultDeltaRot);
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
    if (_deltaColor.isReal()) {
        _deltaColor = _defaultDeltaColor;
    } else {
        _deltaColor = { 0,0,0,0 };
    }
}
