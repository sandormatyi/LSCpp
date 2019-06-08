#include "types.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "colors.h"

const int sz[]{ 256 };
static cv::Mat colors(1, sz, CV_32FC4);


void initColors()
{
}

void mapGreyScaleImage(cv::InputOutputArray image)
{
    cv::Mat colorMap;
    colors.convertTo(colorMap, CV_8UC3);
    cv::applyColorMap(image, image, cv::COLORMAP_OCEAN);
}

void changeColors(float_color_t delta)
{
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 4; ++j) {
            colors.at<float_color_t>(i)[j] = fmod(colors.at<float_color_t>(i)[j] + delta[j], 256.0);
        }
    }
}