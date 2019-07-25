#include <cmath>
#include "keyboardController.h"
#include "../colors.h"
#include "../fractal/fractal.h"
#include "../render/fractalRenderer.h"
#include "../command/allCommands.h"


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
    UIController(fractal, renderer),
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
        sym += 32;
    }

    bool actionHappened = true;

    switch (sym) {
        case 'w':
            zoomIn();
            break;
        case 's':
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
            decreaseResolution();
            break;
        case 'e':
            increaseResolution();
            break;
        case 'a':
            amazingResolution();
            break;
        case 'd':
            shittyResolution();
            break;
        case 'i':
            toggleZoomState(PLUS);
            break;
        case 'o':
            toggleZoomState(MINUS);
            break;
        case 'k':
            toggleNState(PLUS);
            break;
        case 'l':
            toggleNState(MINUS);
            break;
        case 'n':
            toggleRotState(PLUS);
            break;
        case 'm':
            toggleRotState(MINUS);
            break;
        case 'x':
            rotateLeft();
            break;
        case 'c':
            rotateRight();
            break;
        case 'p':
            toggleChangeColorState();
            break;
        case KEY_ESCAPE:
            setQuitFlag();
            break;
        case 'r':
            addCommand(new SetSaveImageCommand(!_renderer.getSaveImage()));
            break;
        case '1':
            addCommand(new SetBlendModeCommand(NO_ALPHA));
            break;
        case '2':
            addCommand(new SetBlendModeCommand(SMOOTH));
            break;
        case '3':           
            addCommand(new SetBlendModeCommand(EPILEPSY));
            break;
        case '4':
            addCommand(new SetBlendModeCommand(SATURATED));
            break;
        case KEY_F1:
            addCommand(new SetTraceModeCommand(DISABLE));
            break;
        case KEY_F2:
            addCommand(new SetTraceModeCommand(PERSIST));
            break;
        case KEY_F3:
            addCommand(new SetTraceModeCommand(FADE_FILLED));
            break;
        case KEY_F4:
            addCommand(new SetTraceModeCommand(FADE_ALL));
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

    if (actionHappened) {
        executeAll();
    }

    return actionHappened;
}

void KeyboardController::moveLeft()
{
    addCommand(new DirectionMoveCommand(LEFT, _defaultMoveSpeed));
}

void KeyboardController::moveRight()
{
    addCommand(new DirectionMoveCommand(RIGHT, _defaultMoveSpeed));
}

void KeyboardController::moveUp()
{
    addCommand(new DirectionMoveCommand(UP, _defaultMoveSpeed));
}

void KeyboardController::moveDown()
{
    addCommand(new DirectionMoveCommand(DOWN, _defaultMoveSpeed));
}

void KeyboardController::zoomIn()
{
    addCommand(new ChangeZoomCommand(_defaultZoomSpeed));
}

void KeyboardController::zoomOut()
{
    addCommand(new ChangeZoomCommand(1 / _defaultZoomSpeed));
}

void KeyboardController::increaseResolution()
{
    addCommand(new ChangeResolutionCommand(_defaultDeltaN));
}

void KeyboardController::decreaseResolution()
{
    addCommand(new ChangeResolutionCommand(-_defaultDeltaN));
}

void KeyboardController::amazingResolution()
{
    addCommand(new SetResolutionCommand(1024));
}

void KeyboardController::shittyResolution()
{
    addCommand(new SetResolutionCommand(8));
}

void KeyboardController::rotateLeft()
{
    addCommand(new ChangeRotationCommand(_defaultDeltaRot));
}

void KeyboardController::rotateRight()
{
    addCommand(new ChangeRotationCommand(-_defaultDeltaRot));
}

void KeyboardController::toggleZoomState(State zoomState)
{
    switch (zoomState) {
        case PLUS:
            if (_zoomSpeed > 1) {
                addCommand(new StopZoomCommand());
            } else {
                addCommand(new StartZoomCommand(_defaultZoomSpeed));
            }
            break;
        case MINUS:
            if (_zoomSpeed < 1) {
                addCommand(new StopZoomCommand());
            } else {
                addCommand(new StartZoomCommand(1 / _defaultZoomSpeed));
            }
            break;
        case ZERO:
            addCommand(new StopZoomCommand());
            break;
    }
}

void KeyboardController::toggleNState(State nState)
{
    switch (nState) {
        case PLUS:
            if (_deltaN > 0) {
                addCommand(new StopChangeResolutionCommand());
            } else {
                addCommand(new StartChangeResolutionCommand(_defaultDeltaN));
            }
            break;
        case MINUS:
            if (_deltaN < 0) {
                addCommand(new StopChangeResolutionCommand());
            } else {
                addCommand(new StartChangeResolutionCommand(-_defaultDeltaN));
            }
            break;
        case ZERO:
            addCommand(new StopChangeResolutionCommand());
            break;
    }
}

void KeyboardController::toggleRotState(State rotState)
{
    switch (rotState) {
        case PLUS:
            if (_deltaRot > 0) {
                addCommand(new StopRotateCommand());
            } else {
                addCommand(new StartRotateCommand(_defaultDeltaRot));
            }
            break;
        case MINUS:
            if (_deltaRot < 0) {
                addCommand(new StopRotateCommand());
            } else {
                addCommand(new StartRotateCommand(-_defaultDeltaRot));
            }
            break;
        case ZERO:
            addCommand(new StopRotateCommand());
            break;
    }
}

void KeyboardController::toggleChangeColorState()
{
    if (_deltaColor.isReal()) {
        addCommand(new StartChangeColorsCommand(_defaultDeltaColor));
    } else {
        addCommand(new StopChangeColorsCommand());
    }
}
