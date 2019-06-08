#pragma once
#include <stdint.h>

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} float_color_t;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} uint8_color_t;

typedef double coord_t;
typedef float fractal_value_t;

enum BlendMode
{
    NO_ALPHA = 0,
    SMOOTH = 1,
    EPILEPSY = 2
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
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Vector2D_t
{
    coord_t x;
    coord_t y;
};