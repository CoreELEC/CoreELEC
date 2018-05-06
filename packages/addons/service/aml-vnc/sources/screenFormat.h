#ifndef SCREEN_FORMAT_H
#define SCREEN_FORMAT_H

#include <stdint.h>

typedef struct _screenFormat
{
	uint16_t width;
	uint16_t height;
	
	uint8_t bitsPerPixel;
	
	uint16_t redMax;
	uint16_t greenMax;
	uint16_t blueMax;
	
	uint8_t redShift;
	uint8_t greenShift;
	uint8_t blueShift;
	
	uint32_t size;
	
	uint32_t pad;
} screenFormat;

#endif
