#ifndef PNGREADER_H_
#define PNGREADER_H_

#include "base_include.h"

#include "IImageReader.h"

#include <png.h>

//--------------------------------------------------
//
// PngReader
//
// Reader for PNG images
//
// Use reader as follows:
//
//		InitReader(data, length);
//		header = ReadHeader();
//
//		// allocate output buffer
//
//		ReadData(buffer);
//
//--------------------------------------------------
class PngReader: public IImageReader {

public:
	PngReader();
	virtual ~PngReader();

	virtual void InitReader(const byte_t* data, size_t length);

	virtual bool ReadHeader(ImageHeader* header);

	virtual bool ReadData(byte_t* buffer);

private:
	png_structp m_Png;
	png_infop m_Info;

	const byte_t* m_SrcBuf;
	size_t m_SrcLength;

	ImageHeader m_Header;

	bool m_Init; // True if the reader has been properly initialized
	bool m_HeaderRead; // True if the header has been read
	bool m_DataRead; // True if the image data has been read
};

#endif