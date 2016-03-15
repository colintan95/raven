#include "PngReader.h"

struct PngBufferRead {
	const byte_t* buffer;
	uint32_t offset;
};

// Used with libpng to track the position in the buffer
static PngBufferRead pngBufRead;

// Read function to use with libpng
void PngReadFunc(png_structp png_ptr, png_bytep data, png_size_t length);


PngReader::PngReader() {

}

PngReader::~PngReader() {

}

void PngReader::InitReader(const byte_t* data, size_t length) {
	ASSERT(!m_Init);

	ASSERT(data != nullptr);
	ASSERT(length > 0);

	// Creates the png_struct
	m_Png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

 	if (!m_Png) {
 		LOG_ERROR("PngReader: Png struct could not be created");
 		m_Init = false;
 		return;
 	}

 	// Creates the png_info
 	m_Info = png_create_info_struct(m_Png);

 	if (!m_Info) {
 		LOG_ERROR("PngReader: Info Struct could not be created");
 		m_Init = false;
 		return;
 	}

 	// Sets the source data to be read
 	m_SrcBuf = data;
 	m_SrcLength = length;

 	// Initializes variables in the reader
 	m_Header.width = 0;
 	m_Header.height = 0;

 	m_Init = true;
 	m_HeaderRead = false;
 	m_DataRead = false;
}

bool PngReader::ReadHeader(ImageHeader* header) {
	ASSERT(m_Init);
	ASSERT(!m_HeaderRead);

	m_Header.width = 0;
	m_Header.height = 0;
	m_Header.colorType = kImageColorNone;
	m_Header.valid = false;

	// Sets the header parameter to invalid by default so that function just 
	// needs to return false on an error
	*header = m_Header;

	pngBufRead.buffer = m_SrcBuf;
	pngBufRead.offset = 0;

	// Fails if the PNG image has an incorrect signature
	if (!png_check_sig(pngBufRead.buffer, 8)) {
		return false;
	}

	// Setup libpng with the png and info struct
	png_set_read_fn(m_Png, (void*)&pngBufRead, PngReadFunc);
	png_read_info(m_Png, m_Info);

	// Deals with errors during header read
	if (setjmp(png_jmpbuf(m_Png))) {
		LOG_ERROR("PngReader: Could not read the PNG image header");
		return false;
	}

	// Reads the data from the PNG header
	size_t width = png_get_image_width(m_Png, m_Info);
	size_t height = png_get_image_height(m_Png, m_Info);
	int colorType = png_get_color_type(m_Png, m_Info);
	int bitDepth = png_get_bit_depth(m_Png, m_Info);

	ASSERT(png_get_interlace_type(m_Png, m_Info) == PNG_INTERLACE_NONE);

	if (bitDepth == 16) {
		LOG_WARNING("PngReader: 16 bit depth not supported; stripped to 8 bit depth");
		png_set_strip_16(m_Png);
		bitDepth = 8;
	}

	if (colorType == PNG_COLOR_TYPE_PALETTE) {
		LOG_ERROR("PngReader: Palette Color Type not supported");
		return false;
	}

	if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
		png_set_expand_gray_1_2_4_to_8(m_Png);
		bitDepth = 8;
	}

	if (png_get_valid(m_Png, m_Info, PNG_INFO_tRNS)) {
		LOG_ERROR("PngReader: tRNS not supported");
		return false;
	}

	switch (colorType) {
		case PNG_COLOR_TYPE_RGB:
			m_Header.colorType = kImageColorRGB;
			m_Header.comp = 3;
			break;
		case PNG_COLOR_TYPE_RGBA:
			m_Header.colorType = kImageColorRGBA;
			m_Header.comp = 4;
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_gray_to_rgb(m_Png);
			m_Header.colorType = kImageColorRGB;
			m_Header.comp = 3;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(m_Png);
			m_Header.colorType = kImageColorRGBA;
			m_Header.comp = 4;
			break;
		default:
			LOG_ERROR("PngReader: Color type not supported");
			return false;
	}

	// Sets the header data
	m_Header.width = width;
	m_Header.height = height;

	m_Header.size = width * height * m_Header.comp;

	m_Header.valid = true;

	*header = m_Header;

	m_HeaderRead = true;

	return true;
}

bool PngReader::ReadData(byte_t* buffer) {
	ASSERT(m_Init);
	ASSERT(m_HeaderRead);
	ASSERT(!m_DataRead);

	// Handles errors during image data read
	if (setjmp(png_jmpbuf(m_Png))) {
		LOG_ERROR("PngReader: Could not read the PNG image data");
		return false;
	}

	size_t rowBytes = png_get_rowbytes(m_Png, m_Info);

	// Invertes the rows because OpenGL textures expects bottom-to-top order
	for (int i = m_Header.height - 1; i >= 0; --i) {
		png_read_row(m_Png, &buffer[i * rowBytes], NULL);
	}

	m_DataRead = true;

	return true;
}


void PngReadFunc(png_structp png_ptr, png_bytep data, png_size_t length) {
	memcpy(data, pngBufRead.buffer + pngBufRead.offset, length);
	pngBufRead.offset += length;
}