#include <SDL_image.h>
#include "externalImage.h"

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *) p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *) p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}

const char *ExternalImage::getFractalName()
{
    return "External image";
}

coord_t ExternalImage::getFractalValue(coord_t mappedX, coord_t mappedY) const
{
    mappedY *= ((coord_t)_surface->w / (coord_t)_surface->h);

    if (mappedX < 0 || mappedY < 0 || mappedX > 1 || mappedY > 1)
        return 0;

    int xPixel = _surface->w * mappedX;
    int yPixel = _surface->h * mappedY;

    Uint8 r, g, b, a;
    Uint32 pixelValue = getPixel(_surface, xPixel, yPixel);
    SDL_GetRGBA(pixelValue, _surface->format, &r, &g, &b, &a);

    coord_t rF, gF, bF;

    rF = r /255.0;
    gF = g / 255.0 + 1;
    bF = b /255.0 + 2;

    return (rF + gF + bF) / 6.0 * _maxN;
}

ExternalImage::ExternalImage(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN, const std::string &imagePath,
                             const SDL_Surface *screenSurface)
        : Fractal(xCenter, yCenter, zoom, maxN)
{
    const std::string path = "../images/" + imagePath;

    //The final optimized image
    SDL_Surface *optimizedSurface = nullptr;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
        if (optimizedSurface == nullptr) {
            SDL_Log("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    _surface = optimizedSurface;
}
