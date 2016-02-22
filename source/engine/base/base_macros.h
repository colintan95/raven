#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

//--------------------------------------------------
//
// Macros used throughout the code
//
//--------------------------------------------------

#include <cassert>

#include "base_define.h"
#include "base_log.h"
#include "base_hash.h"


// Assert macro
#define ASSERT(condition) assert(condition)


// Memory allocation macro
#define MEM_NEW new

#define MEM_DELETE(ptr) delete ptr; ptr = nullptr
#define MEM_DELETE_ARR(ptr) delete[] ptr; ptr = nullptr


// Logging macros

// In debug mode, logging macros will record the file and line information
#if defined(_DEBUG)

// Logs Print, Warning, Error and Fatal Error
#if _LOG_VERBOSE == 1

	#define LOG_PRINT(msg, ...) BaseLogPrintFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)
	#define LOG_WARNING(msg, ...) BaseLogWarningFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)
	#define LOG_ERROR(msg, ...) BaseLogErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)
	#define LOG_FATAL_ERROR(msg, ...) BaseLogFatalErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)

// Logs Warning, Error and Fatal Error
#elif _LOG_VERBOSE == 2

	#define LOG_PRINT(msg, ...)
	#define LOG_WARNING(msg, ...) BaseLogWarningFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)
	#define LOG_ERROR(msg, ...) BaseLogErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)
	#define LOG_FATAL_ERROR(msg, ...) BaseLogFatalErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)

// Logs Error and Fatal Error
#elif _LOG_VERBOSE == 3 

	#define LOG_PRINT(msg, ...)
	#define LOG_WARNING(msg, ...)
	#define LOG_ERROR(msg, ...) BaseLogErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)
	#define LOG_FATAL_ERROR(msg, ...) BaseLogFatalErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)

// Logs Fatal Error only
#elif _LOG_VERBOSE >= 4 

	#define LOG_PRINT(msg, ...)
	#define LOG_WARNING(msg, ...)
	#define LOG_ERROR(msg, ...)
	#define LOG_FATAL_ERROR(msg, ...) BaseLogFatalErrorFileLine(__FILE__, __LINE__, msg, ##__VA_ARGS__)

#endif

// In release mode, only the message is printed
#else

// Logs Fatal Error only
	#define LOG_PRINT(msg, ...) BaseLogPrint(msg, ##__ARGS__)
	#define LOG_WARNING(msg, ...) BaseLogWarning(msg, ##__ARGS__)
	#define LOG_ERROR(msg, ...) BaseLogError(msg, ##__ARGS__)
	#define LOG_FATAL_ERROR(msg, ...) BaseLogFatalError(msg, ##__ARGS__)

#endif


// Stringify macro
#define STRINGIFY(str) #str


// Hash string macro
#define HASH_STRING(str) HashString(str)


// Conversion to bytes
#define MEGABYTES_TO_BYTES(a) a * 1024 * 1024
#define KILOBYTES_TO_BYTES(a) a * 1024


#endif