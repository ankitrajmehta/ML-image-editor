#ifndef OPENER_H
#define OPENER_H

#include <stdint.h> 

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

Pixel* readBMP(const char* filename, int* width, int* height);

void writeBMP(const char* filename, int width, int height, Pixel* pixels);

#endif // OPENER_H