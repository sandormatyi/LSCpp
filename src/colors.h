#ifndef MANDELBROT_COLORS_H
#define MANDELBROT_COLORS_H

#include "types.h"

const int COLOR_NUMBER = 128 * 6;

uint8_color_t to_SDL_Color(const float_color_t &c);


void initColors();
float_color_t &getColor(int index);

int getColorIndex(double f);

uint8_color_t interpolate(const uint8_color_t &c1, const uint8_color_t &c2, double i);
float_color_t interpolate(const float_color_t &c1, const float_color_t &c2, double i);

void changeColors(float deltaR, float deltaG, float deltaB, float deltaA);

bool isEmptyColor(uint8_color_t c);
bool isEmptyColor(float_color_t c);



#endif //MANDELBROT_COLORS_H
