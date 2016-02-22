#include "base_log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>


void BaseLogPrintFileLine(const char* file, int line, const char* fmt, ...) {
    printf("LOG: (file: %s, line: %d) -\n", file, line);

	va_list args;
    va_start(args, fmt);
   	vfprintf(stdout, fmt, args);
    va_end(args);

    printf("\n");
}

void BaseLogWarningFileLine(const char* file, int line, const char* fmt, ...) {
    printf("WARNING: (file: %s, line: %d) -\n", file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    printf("\n");
}

void BaseLogErrorFileLine(const char* file, int line, const char* fmt, ...) {
    printf("ERROR: (file: %s, line: %d) -\n", file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    printf("\n");
}

void BaseLogFatalErrorFileLine(const char* file, int line, const char* fmt, ...) {
    printf("FATAL ERROR: (file: %s, line: %d) -\n", file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    printf("\n");

    exit(EXIT_FAILURE);
}

void BaseLogPrint(const char* fmt, ...) {
    printf("LOG: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

void BaseLogWarning(const char* fmt, ...) {
    printf("WARNING: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

void BaseLogError(const char* fmt, ...) {
    printf("ERROR: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

void BaseLogFatalError(const char* fmt, ...) {
    printf("FATAL ERROR: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}