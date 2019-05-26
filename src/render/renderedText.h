#ifndef MANDELBROT_RENDEREDTEXT_H
#define MANDELBROT_RENDEREDTEXT_H

#include <string>
#include <SDL_render.h>

void initSDLTTF();


struct RenderedText
{
    RenderedText(SDL_Texture *texture, int width, int height)
            : _texture(texture), _width(width), _height(height) {}

    SDL_Texture* _texture;
    int _width;
    int _height;
};

RenderedText textToTexture(const std::string& text, SDL_Renderer *renderer, int textAreaSize);


#endif //MANDELBROT_RENDEREDTEXT_H
