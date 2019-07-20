#include "externalImage.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

const char *ExternalImage::getFractalName()
{
    return "External image";
}

coord_t ExternalImage::getFractalValue(coord_t mappedX, coord_t mappedY) const
{
    mappedY *= ((coord_t)_matrix.cols / (coord_t)_matrix.rows);

    if (mappedX < 0 || mappedY < 0 || mappedX > 1 || mappedY > 1)
        return 0;

    int xPixel = _matrix.cols * mappedX;
    int yPixel = _matrix.rows * mappedY;

    return _matrix.at<uint8_t>(yPixel, xPixel) * _maxN / 255.0;
}

ExternalImage::ExternalImage(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN, const std::string &imagePath)
        : Fractal(xCenter, yCenter, zoom, maxN)
{
    const std::string path = "images/" + imagePath;
    _matrix = cv::imread(path, cv::IMREAD_GRAYSCALE);
}