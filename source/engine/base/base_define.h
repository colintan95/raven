#ifndef BASE_DEFINE_H_
#define BASE_DEFINE_H_

//--------------------------------------------------
//
// base_define.h
//
// Define macros
//
// Macros of type EXT_* are defined from CMake
//
//--------------------------------------------------


// Define if debug mode

#undef _DEBUG

#if defined(EXT_DEBUG_MODE)
	#define _DEBUG
#endif


// etermines the operating system

#undef _PLATFORM_WIN
#undef _PLATFORM_OSX
#undef _PLATFORM_LINUX

#if defined(EXT_PLATFORM_WIN)
	#define _PLATFORM_WIN
#elif defined(EXT_PLATFORM_OSX)
	#define _PLATFORM_OSX
#elif defined(EXT_PLATFORM_LINUX)
	#define _PLATFORM_LINUX
#endif


// Sets the verbosity for logs
// Default verbosity level is 1 (highest)

#if defined(EXT_LOG_VERBOSE) && EXT_LOG_VERBOSE > 0
	#define _LOG_VERBOSE EXT_LOG_VERBOSE
#else
	#define _LOG_VERBOSE 1
#endif

#endif