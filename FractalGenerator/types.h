#pragma once
#include <stdint.h>
#include <opencv2/core/types.hpp>

typedef cv::Scalar_<float> float_color_t;
typedef cv::Scalar_<uint8_t> uint8_color_t;

typedef double coord_t;
typedef float fractal_value_t;

enum BlendMode
{
    NO_ALPHA = 0,
    SMOOTH = 1,
    EPILEPSY = 2,
    SATURATED = 3
};

enum TraceMode
{
    DISABLE = 0,
    PERSIST = 1,
    FADE_FILLED = 2,
    FADE_ALL = 3
};

enum Direction
{
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
};

struct Vector2D_t
{
    coord_t x;
    coord_t y;
};