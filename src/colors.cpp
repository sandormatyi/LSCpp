#include <stdint.h>
#include <cmath>
#include "colors.h"
#include "fractal/fractal.h"


uint8_color_t to_SDL_Color(const float_color_t &c)
{
    return {
            (uint8_t) fmod(c.r, 256.0),
            (uint8_t) fmod(c.g, 256.0),
            (uint8_t) fmod(c.b, 256.0),
            (uint8_t) fmod(c.a, 256.0)
    };
}

float_color_t colors[COLOR_NUMBER];

float interpolate(float start, float end, float value)
{
    return (value * (end - start) + start);
}

float_color_t interpolate(const float_color_t &c1, const float_color_t &c2, double i)
{
    float_color_t c;
    c.r = ((c1.r * (1.0 - i)) + (c2.r * i));
    c.g = ((c1.g * (1.0 - i)) + (c2.g * i));
    c.b = ((c1.b * (1.0 - i)) + (c2.b * i));
    c.a = ((c1.a * (1.0 - i)) + (c2.a * i));

    return c;
}

uint8_color_t interpolate(const uint8_color_t &c1, const uint8_color_t &c2, double i)
{
    uint8_color_t c;
    c.r = (uint8_t) ((c1.r * (1.0 - i)) + (c2.r * i));
    c.g = (uint8_t) ((c1.g * (1.0 - i)) + (c2.g * i));
    c.b = (uint8_t) ((c1.b * (1.0 - i)) + (c2.b * i));
    c.a = (uint8_t) ((c1.a * (1.0 - i)) + (c2.a * i));

    return c;
}

void initRGB()
{
    const int COLOR_NUMBER_6 = COLOR_NUMBER / 6;

#define VAL ((float)i / (float)COLOR_NUMBER_6)

    for (int i = 0; i < COLOR_NUMBER_6; ++i) {
        colors[i].r = 255;
        colors[i].g = interpolate(0, 255, VAL);
        colors[i].b = 0;
        colors[i].a = 0xFF;
    }

    for (int i = 0; i < COLOR_NUMBER_6; ++i) {
        colors[i + COLOR_NUMBER_6].r = interpolate(255, 0, VAL);
        colors[i + COLOR_NUMBER_6].g = 255;
        colors[i + COLOR_NUMBER_6].b = 0;
        colors[i + COLOR_NUMBER_6].a = 0xFF;
    }

    for (int i = 0; i < COLOR_NUMBER_6; ++i) {
        colors[i + COLOR_NUMBER_6 * 2].r = 0;
        colors[i + COLOR_NUMBER_6 * 2].g = 255;
        colors[i + COLOR_NUMBER_6 * 2].b = interpolate(0, 255, VAL);
        colors[i + COLOR_NUMBER_6 * 2].a = 0xFF;
    }

    for (int i = 0; i < COLOR_NUMBER_6; ++i) {
        colors[i + COLOR_NUMBER_6 * 3].r = 0;
        colors[i + COLOR_NUMBER_6 * 3].g = interpolate(255, 0, VAL);
        colors[i + COLOR_NUMBER_6 * 3].b = 255;
        colors[i + COLOR_NUMBER_6 * 3].a = 0xFF;
    }

    for (int i = 0; i < COLOR_NUMBER_6; ++i) {
        colors[i + COLOR_NUMBER_6 * 4].r = interpolate(0, 255, VAL);
        colors[i + COLOR_NUMBER_6 * 4].g = 0;
        colors[i + COLOR_NUMBER_6 * 4].b = 255;
        colors[i + COLOR_NUMBER_6 * 4].a = 0xFF;
    }

    for (int i = 0; i < COLOR_NUMBER_6; ++i) {
        colors[i + COLOR_NUMBER_6 * 5].r = 255;
        colors[i + COLOR_NUMBER_6 * 5].g = 0;
        colors[i + COLOR_NUMBER_6 * 5].b = interpolate(255, 0, VAL);
        colors[i + COLOR_NUMBER_6 * 5].a = 0xFF;
    }
}

void initColors()
{

    /* for (int i = 0; i < COLOR_NUMBER; ++i) {
         colors[i].r = interpolate(0xFF, 0x00, float(i)/(float)COLOR_NUMBER);
         colors[i].g = interpolate(0xFF, 0x00, float(i)/(float)COLOR_NUMBER);
         colors[i].b = interpolate(0xFF, 0x00, float(i)/(float)COLOR_NUMBER);
         colors[i].a = SDL_ALPHA_OPAQUE;
     }
     */
    initRGB();
    /*
        colors[0].r = 0;
        colors[0].g = 0;
        colors[0].b = 0;
        colors[0].a = 0xFF;
         */
}

float_color_t &getColor(int index)
{
    if (index >= COLOR_NUMBER)
        index = COLOR_NUMBER - 1;

    return colors[index];
}


int getColorIndex(double f)
{
    if (f > 1.0)
        f = 1.0;

    int index = (int)std::floor((double)(f * (double) COLOR_NUMBER));
    return index;
}

void changeColors(float deltaR, float deltaG, float deltaB, float deltaA)
{
    for (int i = 0; i < COLOR_NUMBER; ++i) {
        colors[i].r = fmod(colors[i].r + deltaR, 256.0);
        colors[i].g = fmod(colors[i].g + deltaG, 256.0);
        colors[i].b = fmod(colors[i].b + deltaB, 256.0);
        colors[i].a = fmod(colors[i].a + deltaA, 256.0);
    }
}

bool isEmptyColor(uint8_color_t c)
{
    return c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0;
}

bool isEmptyColor(float_color_t c)
{
    return c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0;
}