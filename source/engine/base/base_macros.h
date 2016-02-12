#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

//--------------------------------------------------
//
// base_macros.h
//
// Macros used throughout the code
//
//--------------------------------------------------

#include <cassert>

#include "base_define.h"
#include "base_log.h"


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
	#define LOG_WARNING(msg, ...) // add warning function
	#define LOG_ERROR(msg, ...) // add error function
	#define LOG_FATAL_ERROR(msg, ...) // add fatal error function

// Logs Warning, Error and Fatal Error
#elif _LOG_VERBOSE == 2

	#define LOG_PRINT(msg, ...)
	#define LOG_WARNING(msg, ...) // add warning function
	#define LOG_ERROR(msg, ...) // add error function
	#define LOG_FATAL_ERROR(msg, ...) // add fatal error function

// Logs Error and Fatal Error
#elif _LOG_VERBOSE == 3 

	#define LOG_PRINT(msg, ...)
	#define LOG_WARNING(msg, ...)
	#define LOG_ERROR(msg, ...) // add error function
	#define LOG_FATAL_ERROR(msg, ...) // add fatal error function

// Logs Fatal Error only
#elif _LOG_VERBOSE >= 4 

	#define LOG_PRINT(msg, ...)
	#define LOG_WARNING(msg, ...)
	#define LOG_ERROR(msg, ...)
	#define LOG_FATAL_ERROR(msg, ...) // add fatal error function

#endif

// In release mode, only the message is printed
#else

// Logs Fatal Error only
	#define LOG_PRINT(msg, ...) BaseLogPrint(msg, ##__ARGS__)
	#define LOG_WARNING(msg, ...)
	#define LOG_ERROR(msg, ...)
	#define LOG_FATAL_ERROR(msg, ...) // add fatal error function

#endif



#endif