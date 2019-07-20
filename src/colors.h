#ifndef MANDELBROT_COLORS_H
#define MANDELBROT_COLORS_H

#include "types.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void initColors();
void mapGreyScaleImageToBGRA(cv::InputOutputArray image, cv::ColormapTypes colorMap);
void changeColors(float_color_t delta);

#endif //MANDELBROT_COLORS_H
