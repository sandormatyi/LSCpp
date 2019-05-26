#include <SDL_log.h>
#include <SDL_ttf.h>
#include "renderedText.h"

TTF_Font *_font = nullptr;

RenderedText textToTexture(const std::string &text, SDL_Renderer *renderer, int textAreaSize)
{
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( _font, text.c_str(), textColor, textAreaSize);
    if( textSurface == nullptr )
    {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR);
        SDL_Log( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return {nullptr, 0, 0};
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
    if( texture == nullptr )
    {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR);
        SDL_Log( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        return {nullptr, 0, 0};
    }


    RenderedText renderedText(texture, textSurface->w, textSurface->h);
    SDL_FreeSurface(textSurface);

    return renderedText;
}

void initSDLTTF()
{
    if( TTF_Init() == -1 )
    {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR);
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    _font = TTF_OpenFont( "lazy.ttf", 18 );
    if( _font == nullptr )
    {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR);
        SDL_Log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}
