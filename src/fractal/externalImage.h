#ifndef MANDELBROT_EXTERNALIMAGE_H
#define MANDELBROT_EXTERNALIMAGE_H


#include <SDL_render.h>
#include <string>
#include "fractal.h"

class ExternalImage : public Fractal
{
public:
    ExternalImage(coord_t xCenter, coord_t yCenter, coord_t zoom, coord_t maxN, const std::string &imagePath,
                  const SDL_Surface *);
    const char *getFractalName() override;

protected:
    coord_t getFractalValue(coord_t mappedX, coord_t mappedY) const override;

private:
    SDL_Surface *_surface;
};


#endif //MANDELBROT_EXTERNALIMAGE_H
