#include "renderedText.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>


void textToTexture(const std::string& text, cv::InputOutputArray texture, int textAreaSize)
{
    std::stringstream ss(text);
    std::string line;
    int y = 25;
    while (!std::getline(ss, line).eof()) {
        cv::putText(texture, line, cv::Point(0, y), cv::FONT_HERSHEY_DUPLEX, 0.6, 0);
        y += 25;
    }
}

/*
RenderedText textToTexture(const std::string &text, SDL_Renderer *renderer, int textAreaSize)
{
    uint8_color_t textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( _font, text.c_str(), textColor, textAreaSize);
    if( textSurface == nullptr )
    {
        std::cout <<( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return {nullptr, 0, 0};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
    if( texture == nullptr )
    {
        SDL_Log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        return {nullptr, 0, 0};
    }
    

    RenderedText renderedText(texture, textSurface->w, textSurface->h);
    SDL_FreeSurface(textSurface);

    return renderedText;
}
*/