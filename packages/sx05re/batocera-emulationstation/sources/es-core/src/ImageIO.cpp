#include "ImageIO.h"

#include "Log.h"
#include <FreeImage.h>
#include <string.h>
#include "utils/FileSystemUtil.h"
#include "utils/StringUtil.h"

unsigned char* ImageIO::loadFromMemoryRGBA32(const unsigned char * data, const size_t size, size_t & width, size_t & height, MaxSizeInfo* maxSize, Vector2i* baseSize, Vector2i* packedSize)
{
	if (baseSize != nullptr)
		*baseSize = Vector2i(0, 0);

	if (baseSize != nullptr)
		*packedSize = Vector2i(0, 0);

	std::vector<unsigned char> rawData;
	width = 0;
	height = 0;
	FIMEMORY * fiMemory = FreeImage_OpenMemory((BYTE *)data, (DWORD)size);
	if (fiMemory != nullptr) 
	{
		//detect the filetype from data
		FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(fiMemory);
		if (format != FIF_UNKNOWN && FreeImage_FIFSupportsReading(format))
		{
			//file type is supported. load image
			FIBITMAP * fiBitmap = FreeImage_LoadFromMemory(format, fiMemory);
			if (fiBitmap != nullptr)
			{
				//loaded. convert to 32bit if necessary
				if (FreeImage_GetBPP(fiBitmap) != 32)
				{
					FIBITMAP * fiConverted = FreeImage_ConvertTo32Bits(fiBitmap);
					if (fiConverted != nullptr)
					{
						//free original bitmap data
						FreeImage_Unload(fiBitmap);
						fiBitmap = fiConverted;
					}
				}
				if (fiBitmap != nullptr)
				{
					width = FreeImage_GetWidth(fiBitmap);
					height = FreeImage_GetHeight(fiBitmap);

					if (baseSize != nullptr)
						*baseSize = Vector2i(width, height);

					if (maxSize != nullptr && maxSize->x() > 0 && maxSize->y() > 0 && (width > maxSize->x() || height > maxSize->y()))
					{
						Vector2i sz = adjustPictureSize(Vector2i(width, height), Vector2i(maxSize->x(), maxSize->y()), maxSize->externalZoom());
						if (sz.x() != width || sz.y() != height)
						{
							FIBITMAP* imageRescaled = FreeImage_Rescale(fiBitmap, sz.x(), sz.y(), FILTER_BOX);
							FreeImage_Unload(fiBitmap);
							fiBitmap = imageRescaled;

							width = FreeImage_GetWidth(fiBitmap);
							height = FreeImage_GetHeight(fiBitmap);

							if (packedSize != nullptr)
								*packedSize = Vector2i(width, height);
						}
					}

					unsigned char* tempData = new unsigned char[width * height * 4];

					int w = (int)width;

					for (int y = (int)height; --y >= 0; )
					{
						unsigned int* argb = (unsigned int*)FreeImage_GetScanLine(fiBitmap, y);
						unsigned int* abgr = (unsigned int*)(tempData + (y * width * 4));
						for (int x = w; --x >= 0;)
						{
							unsigned int c = argb[x];
							abgr[x] = (c & 0xFF00FF00) | ((c & 0xFF) << 16) | ((c >> 16) & 0xFF);
						}
					}

					FreeImage_Unload(fiBitmap);
					FreeImage_CloseMemory(fiMemory);

					return tempData;
				}
			}
			else
			{
				LOG(LogError) << "Error - Failed to load image from memory!";
			}
		}
		else
		{
			LOG(LogError) << "Error - File type " << (format == FIF_UNKNOWN ? "unknown" : "unsupported") << "!";
		}
		//free FIMEMORY again
		FreeImage_CloseMemory(fiMemory);
	}

	return nullptr;
}

void ImageIO::flipPixelsVert(unsigned char* imagePx, const size_t& width, const size_t& height)
{
	unsigned int temp;
	unsigned int* arr = (unsigned int*)imagePx;
	for(size_t y = 0; y < height / 2; y++)
	{
		for(size_t x = 0; x < width; x++)
		{
			temp = arr[x + (y * width)];
			arr[x + (y * width)] = arr[x + (height * width) - ((y + 1) * width)];
			arr[x + (height * width) - ((y + 1) * width)] = temp;
		}
	}
}

Vector2i ImageIO::adjustPictureSize(Vector2i imageSize, Vector2i maxSize, bool externSize)
{
	if (externSize)
	{
		Vector2f szf = getPictureMinSize(Vector2f(imageSize.x(), imageSize.y()), Vector2f(maxSize.x(), maxSize.y()));
		return Vector2i(szf.x(), szf.y());
	}

	int cxDIB = imageSize.x();
	int cyDIB = imageSize.y();
	int iMaxX = maxSize.x();
	int iMaxY = maxSize.y();

	double xCoef = (double)iMaxX / (double)cxDIB;
	double yCoef = (double)iMaxY / (double)cyDIB;

#if WIN32
	cyDIB = (int)((double)cyDIB * std::fmax(xCoef, yCoef));
	cxDIB = (int)((double)cxDIB * std::fmax(xCoef, yCoef));
#else
	cyDIB = (int)((double)cyDIB * std::max(xCoef, yCoef));
	cxDIB = (int)((double)cxDIB * std::max(xCoef, yCoef));
#endif

	if (cxDIB > iMaxX)
	{
		cyDIB = (int)((double)cyDIB * (double)iMaxX / (double)cxDIB);
		cxDIB = iMaxX;
	}

	if (cyDIB > iMaxY)
	{
		cxDIB = (int)((double)cxDIB * (double)iMaxY / (double)cyDIB);
		cyDIB = iMaxY;
	}

	return Vector2i(cxDIB, cyDIB);
}

Vector2f ImageIO::getPictureMinSize(Vector2f imageSize, Vector2f maxSize)
{
	float cxDIB = maxSize.x();
	float cyDIB = maxSize.y();

	float xCoef = maxSize.x() / imageSize.x();
	float yCoef = maxSize.y() / imageSize.y();

	if (imageSize.x() * yCoef < maxSize.x())
		cyDIB = imageSize.y() * xCoef;
	else
		cxDIB = imageSize.x() * yCoef;

	return Vector2f(cxDIB, cyDIB);
}


bool ImageIO::loadImageSize(const char *fn, unsigned int *x, unsigned int *y)
{
	auto ext = Utils::String::toLower(Utils::FileSystem::getExtension(fn));
	if (ext != ".jpg" && ext != ".png" && ext != ".jpeg" && ext != ".gif")
		return false;

	FILE *f = fopen(fn, "rb");
	if (f == 0)
		return false;

	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (len < 24) {
		fclose(f);
		return false;
	}

	// Strategy:
	// reading GIF dimensions requires the first 10 bytes of the file
	// reading PNG dimensions requires the first 24 bytes of the file
	// reading JPEG dimensions requires scanning through jpeg chunks
	// In all formats, the file is at least 24 bytes big, so we'll read that always
	unsigned char buf[24]; fread(buf, 1, 24, f);


	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.

	if ((buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F') ||
		(buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE1 && buf[6] == 'E' && buf[7] == 'x' && buf[8] == 'i' && buf[9] == 'f'))
	{
		long pos = 2;
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
				break;

			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len) break;
			fseek(f, pos, SEEK_SET);
			fread(buf + 2, 1, 12, f);
		}
	}

	fclose(f);

	// JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
	{
		*y = (buf[7] << 8) + buf[8];
		*x = (buf[9] << 8) + buf[10];

		if (*x > 5000) // security ?
			return false;

		return true;
	}

	// GIF: first three bytes say "GIF", next three give version number. Then dimensions
	if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		*x = buf[6] + (buf[7] << 8);
		*y = buf[8] + (buf[9] << 8);
		return true;
	}

	// PNG: the first frame is by definition an IHDR frame, which gives dimensions
	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		*x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);

		return true;
	}

	return false;
}
