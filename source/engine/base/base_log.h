#ifndef BASE_LOG_H_
#define BASE_LOG_H_

//--------------------------------------------------
//
// Defines the logging functions
//
// User should call the logging functions using the macros from base_include.h
//
//--------------------------------------------------

// Log functions with file and line information
void BaseLogPrintFileLine(const char* file, int line, const char* fmt, ...);
void BaseLogWarningFileLine(const char* file, int line, const char* fmt, ...);
void BaseLogErrorFileLine(const char* file, int line, const char* fmt, ...);
void BaseLogFatalErrorFileLine(const char* file, int line, const char* fmt, ...);

// Log functions without file and line information
void BaseLogPrint(const char* fmt, ...);
void BaseLogWarning(const char* fmt, ...);
void BaseLogError(const char* fmt, ...);
void BaseLogFatalError(const char* fmt, ...);

#endif