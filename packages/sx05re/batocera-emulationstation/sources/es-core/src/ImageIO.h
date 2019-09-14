#pragma once
#ifndef ES_CORE_IMAGE_IO
#define ES_CORE_IMAGE_IO

#include <stdlib.h>
#include <vector>
#include "math/Vector2f.h"
#include "math/Vector2i.h"


class MaxSizeInfo
{
public:
	MaxSizeInfo() : mSize(Vector2f(0, 0)), mExternalZoom(false) {}

	MaxSizeInfo(float x, float y) : mSize(Vector2f(x, y)), mExternalZoom(false), mExternalZoomKnown(false) { }
	MaxSizeInfo(Vector2f size) : mSize(size), mExternalZoom(false), mExternalZoomKnown(false) { }

	MaxSizeInfo(float x, float y, bool externalZoom) : mSize(Vector2f(x, y)), mExternalZoom(externalZoom), mExternalZoomKnown(true) { }
	MaxSizeInfo(Vector2f size, bool externalZoom) : mSize(size), mExternalZoom(externalZoom), mExternalZoomKnown(true) { }

	bool empty() { return mSize.x() <= 1 && mSize.y() <= 1; }

	float x() { return mSize.x(); }
	float y() { return mSize.y(); }

	bool externalZoom()
	{
		return mExternalZoom;
	}

	bool isExternalZoomKnown()
	{
		return mExternalZoomKnown;
	}

private:
	Vector2f mSize;
	bool	 mExternalZoom;
	bool	 mExternalZoomKnown;
};

class ImageIO
{
public:
	static unsigned char*  loadFromMemoryRGBA32(const unsigned char * data, const size_t size, size_t & width, size_t & height, MaxSizeInfo* maxSize = nullptr, Vector2i* baseSize = nullptr, Vector2i* packedSize = nullptr);
	static void flipPixelsVert(unsigned char* imagePx, const size_t& width, const size_t& height);

	// batocera
	static Vector2f getPictureMinSize(Vector2f imageSize, Vector2f maxSize);
	static Vector2i adjustPictureSize(Vector2i imageSize, Vector2i maxSize, bool externSize = false);
	static bool		loadImageSize(const char *fn, unsigned int *x, unsigned int *y);
};

#endif // ES_CORE_IMAGE_IO
