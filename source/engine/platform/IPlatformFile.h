#ifndef IPLATFORMFILE_H_
#define IPLATFORMFILE_H_

#include "base_include.h"

enum PlatformFileMode_t {
	kPlatformFileNull,
	kPlatformFileRead,
	kPlatformFileWrite,
	kPlatformFileAppend
};

//--------------------------------------------------
//
// IPlatformFile
//
// Interface for PlatformFile
//
// Implementations are platform specific; found in the platform source folders
//
//--------------------------------------------------
class IPlatformFile {

public:
	virtual ~IPlatformFile() {}

	virtual bool Open(const char* name, PlatformFileMode_t mode) = 0;
	virtual void Close() = 0;

	virtual size_t GetSize() const = 0;
	virtual bool IsOpen() const = 0;

};

#endif