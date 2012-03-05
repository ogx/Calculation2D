//	This class Copyright 1997, Chris Losinger
//	This is free to use and modify provided my name is
//	included.
#pragma once
#include <mmGlobalDefs.h>
#include <windows.h>

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define BMP_HEADERSIZE (3 * 2 + 4 * 12)
#define BMP_BYTESPERLINE (width, bits) ((((width) * (bits) + 31) / 32) * 4)
#define BMP_PIXELSIZE(width, height, bits) (((((width) * (bits) + 31) / 32) * 4) * height)

namespace Bitmaps
{
		unsigned char * LoadBMP(mmString p_sFileName, mmInt * p_iWidth, mmInt * p_iHeight, mmInt * p_iDepth);
		void GetBMPInfo(mmString p_sFileName, mmInt * p_iWidth, mmInt * p_iHeight, mmInt * p_iDepth);
		bool SaveBMP(mmString p_sFileName, const unsigned char *  p_pucData, mmInt p_iWidth, mmInt p_iHeight);
		void SaveBMP(mmString   fileName, unsigned char * colormappedbuffer, mmInt width, mmInt height, mmInt bitsperpixel, mmInt colors, RGBQUAD *colormap);
};