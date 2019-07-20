#ifndef MANDELBROT_COLORS_H
#define MANDELBROT_COLORS_H

#include "types.h"
#include <opencv2/core.hpp>

void initColors();
void mapGreyScaleImageToBGRA(cv::InputOutputArray image);
void changeColors(float_color_t delta);
void changeColorMap(int colorMap);

#endif //MANDELBROT_COLORS_H