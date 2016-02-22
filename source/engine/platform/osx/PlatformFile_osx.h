#ifndef PLATFORMFILE_OSX_H_
#define PLATFORMFILE_OSX_H_

#include "base_include.h"

#include "../IPlatformFile.h"

// Forward declarations
class PlatformFileSystemImpl;

//--------------------------------------------------
//
// PlatformFile
//
// OSX specfic implementation of PlatformFile
//
//--------------------------------------------------
class PlatformFile: public IPlatformFile {
	friend class PlatformFileSystemImpl;

public:
	PlatformFile();
	~PlatformFile();

	bool Open(const char* name, PlatformFileMode_t mode);
	void Close();

	size_t GetSize() const { return m_Size; }
	bool IsOpen() const { return (m_FileDes != -1); }

private:
	int m_FileDes;

	PlatformFileMode_t m_Mode;

	size_t m_Size;
};


#endif