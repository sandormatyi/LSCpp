#ifndef MANDELBROT_RENDEREDTEXT_H
#define MANDELBROT_RENDEREDTEXT_H

#include <string>
#include <opencv2/core/core.hpp>

void textToTexture(const std::string& text, cv::InputOutputArray texture, int textAreaSize);


#endif //MANDELBROT_RENDEREDTEXT_H
