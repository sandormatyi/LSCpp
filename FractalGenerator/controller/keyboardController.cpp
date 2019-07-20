#include <cmath>
#include "keyboardController.h"
#include "../colors.h"
#include "../fractal/fractal.h"
#include "../render/fractalRenderer.h"
#include "../command/allCommands.h"


KeyboardController::KeyboardController(Fractal &fractal, FractalRenderer &renderer, coord_t defaultZoomSpeed,
    coord_t defaultMoveSpeed, fractal_value_t defaultDeltaN, coord_t defaultDeltaRot,
    float_color_t defaultDeltaColor) :
    UIController(fractal, renderer),
    _defaultZoomSpeed(defaultZoomSpeed),
    _defaultMoveSpeed(defaultMoveSpeed),
    _defaultDeltaN(defaultDeltaN),
    _defaultDeltaRot(defaultDeltaRot),
    _defaultDeltaColor(defaultDeltaColor)
{
}

bool KeyboardController::processKeyboardInput(Keys key, bool shift)
{
    const coord_t multiplier = 10;

    coord_t zoomSpeed = _defaultZoomSpeed;
    coord_t moveSpeed = _defaultMoveSpeed;
    coord_t deltaRot = _defaultDeltaRot;
    fractal_value_t deltaN = _defaultDeltaN;

    if (shift) {
        _defaultZoomSpeed *= multiplier;
        _defaultMoveSpeed *= multiplier;
        _defaultDeltaRot *= multiplier;
        _defaultDeltaN *= multiplier;
    }

    bool actionHappened = true;

    switch (key) {
        case Keys::E:
            zoomIn();
            break;
        case Keys::Q:
            zoomOut();
            break;
        case Keys::W:
            moveUp();
            break;
        case Keys::S:
            moveDown();
            break;
        case Keys::A:
            moveLeft();
            break;
        case Keys::D:
            moveRight();
            break;
        case Keys::Down:
            decreaseResolution();
            break;
        case Keys::Up:
            increaseResolution();
            break;
        case Keys::Right:
            amazingResolution();
            break;
        case Keys::Left:
            shittyResolution();
            break;
        case Keys::I:
            toggleZoomState(PLUS);
            break;
        case Keys::O:
            toggleZoomState(MINUS);
            break;
        case Keys::K:
            toggleNState(PLUS);
            break;
        case Keys::L:
            toggleNState(MINUS);
            break;
        case Keys::N:
            toggleRotState(PLUS);
            break;
        case Keys::M:
            toggleRotState(MINUS);
            break;
        case Keys::X:
            rotateLeft();
            break;
        case Keys::C:
            rotateRight();
            break;
        case Keys::P:
            toggleChangeColorState();
            break;
        case Keys::Escape:
            setQuitFlag();
            break;
     //   case Keys::R:
     //       addCommand(new SetSaveImageCommand(!_renderer.getSaveImage()));
     //       break;
        case Keys::D1:
            addCommand(new SetBlendModeCommand(NO_ALPHA));
            break;
        case Keys::D2:
            addCommand(new SetBlendModeCommand(SMOOTH));
            break;
        case Keys::D3:
            addCommand(new SetBlendModeCommand(EPILEPSY));
            break;
        case Keys::D4:
            addCommand(new SetBlendModeCommand(SATURATED));
            break;
        case Keys::F1:
            addCommand(new SetTraceModeCommand(DISABLE));
            break;
        case Keys::F2:
            addCommand(new SetTraceModeCommand(PERSIST));
            break;
        case Keys::F3:
            addCommand(new SetTraceModeCommand(FADE_FILLED));
            break;
        case Keys::F4:
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

    if (shift) {
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

void KeyboardController::setDefaultSpeeds(coord_t zoomSpeed, coord_t moveSpeed, fractal_value_t deltaResolution, coord_t deltaRot, float_color_t deltaColor)
{
    _defaultZoomSpeed = zoomSpeed;
    _defaultMoveSpeed = moveSpeed;
    _defaultDeltaN = deltaResolution;
    _defaultDeltaRot = deltaRot;
    _defaultDeltaColor = deltaColor;
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
