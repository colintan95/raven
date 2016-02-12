#ifndef BASE_LOG_H_
#define BASE_LOG_H_

//--------------------------------------------------
//
// base_log.h
//
// Defines the logging functions
//
// User should call the logging functions using the macros from base_include.h
//
//--------------------------------------------------

// Log functions with file and line information
void BaseLogPrintFileLine(const char* file, int line, const char* fmt, ...);

// Log functions without file and line information
void BaseLogPrint(const char* fmt, ...);


#endif