#ifndef MANDELBROT_RENDERPARAMS_H
#define MANDELBROT_RENDERPARAMS_H

const size_t SCREEN_WIDTH = 256*5;
const size_t SCREEN_HEIGHT = 144*5;

const double FRAMES_PER_SECOND = 30;

const size_t TEXTURE_WIDTH = SCREEN_WIDTH;
const size_t TEXTURE_HEIGHT = SCREEN_HEIGHT;

const int THREAD_NUMBER = 16;

static_assert(SCREEN_HEIGHT % THREAD_NUMBER == 0, "Screen height is not divisible by thread number!");
static_assert(SCREEN_WIDTH % THREAD_NUMBER == 0, "Screen width is not divisible by thread number!");
static_assert(TEXTURE_HEIGHT % THREAD_NUMBER == 0, "Texture height is not divisible by thread number!");
static_assert(TEXTURE_WIDTH % THREAD_NUMBER == 0, "Texture width is not divisible by thread number!");

#endif //MANDELBROT_RENDERPARAMS_H
