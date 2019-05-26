#ifndef MANDELBROT_COLORS_H
#define MANDELBROT_COLORS_H

#include <SDL_pixels.h>

const int COLOR_NUMBER = 128 * 6;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} float_color_t;

SDL_Color to_SDL_Color(const float_color_t &c);


void initColors();
float_color_t &getColor(int index);

int getColorIndex(double f);

SDL_Color interpolate(const SDL_Color &c1, const SDL_Color &c2, double i);
float_color_t interpolate(const float_color_t &c1, const float_color_t &c2, double i);

void changeColors(float deltaR, float deltaG, float deltaB, float deltaA);

bool isEmptyColor(SDL_Color c);
bool isEmptyColor(float_color_t c);



#endif //MANDELBROT_COLORS_H
