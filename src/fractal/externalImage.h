#ifndef MANDELBROT_EXTERNALIMAGE_H
#define MANDELBROT_EXTERNALIMAGE_H


#include <string>
#include <opencv2/core.hpp>
#include "fractal.h"

class ExternalImage : public Fractal
{
public:
    ExternalImage(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN, const std::string &imagePath);
    const char *getFractalName() override;

protected:
    coord_t getFractalValue(coord_t mappedX, coord_t mappedY) const override;

private:
    cv::Mat _matrix;
};


#endif //MANDELBROT_EXTERNALIMAGE_H
