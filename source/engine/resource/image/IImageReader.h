#ifndef IIMAGE_READER_H_
#define IIMAGE_READER_H_

#include "base_include.h"

enum ImageColorType_t {
	kImageColorNone,
	kImageColorRGB,
	kImageColorRGBA
};

struct ImageHeader {
	size_t width;
	size_t height;

	size_t size;
	size_t comp; // Number of color components

	ImageColorType_t colorType;

	bool valid; // True if the header is valid
};

//--------------------------------------------------
//
// IImageReader
//
// Interface for all image readers
//
// All images are converted to RGBA images
//
//--------------------------------------------------
class IImageReader {

public:
	virtual ~IImageReader() {}

	// Must be called before any other operations
	virtual void InitReader(const byte_t* data, size_t length) = 0;

	// Outputs the header information to the parameter
	//
	// On error, the function returns false, and the header is made invalid
	virtual bool ReadHeader(ImageHeader* header) = 0;

	// outBuf must be large enough to fit the uncompressed image data
	virtual bool ReadData(byte_t* buffer) = 0;

};

#endif