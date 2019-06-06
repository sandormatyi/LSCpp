#include "renderedText.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>


void textToTexture(const std::string& text, cv::InputOutputArray texture)
{
    std::stringstream ss(text);
    std::string line;
    int y = 25;
    while (!std::getline(ss, line).eof()) {
        cv::putText(texture, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, 0, 2);
        cv::putText(texture, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, { 255, 255, 255 });
        y += 25;
    }
}