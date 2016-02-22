#include "PlatformFile_osx.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

PlatformFile::PlatformFile() {
	m_FileDes = -1;

	m_Mode = kPlatformFileNull;
	m_Size = 0;
}

PlatformFile::~PlatformFile() {
	if (m_FileDes != -1) {
		Close();
	}

	m_Mode = kPlatformFileNull;
	m_Size = 0;
}

bool PlatformFile::Open(const char* name, PlatformFileMode_t mode) {
	// File is already open
	if (m_FileDes != -1) {
		return false;
	}

	int flags = 0;

	if (mode == kPlatformFileRead) {
		flags = O_RDONLY;
	}
	else if (mode == kPlatformFileWrite) {
		flags = O_WRONLY | O_TRUNC | O_CREAT;
	}
	else if (mode == kPlatformFileAppend) {
		flags = O_WRONLY | O_CREAT | O_APPEND;
	}
	else {
		ASSERT(0);
	}

	m_Mode = mode;

	mode_t permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	m_FileDes = open(name, flags, permissions);

	if (m_FileDes == -1) {
		return false;
	}

	struct stat fileStat;

	int statRes = fstat(m_FileDes, &fileStat);

	if (statRes == -1) {
		ASSERT(0); 
	}

	m_Size = fileStat.st_size;

	return true;
}

void PlatformFile::Close() {
	if (m_FileDes != -1) {
		close(m_FileDes);
	}

	m_FileDes = -1;

	m_Mode = kPlatformFileNull;
	m_Size = 0;
}