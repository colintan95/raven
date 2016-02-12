#include "base_log.h"

#include <cstdarg>
#include <cstdio>


void BaseLogPrintFileLine(const char* file, int line, const char* fmt, ...) {
	//BaseLogPrintInternal("LOG: (file: %s, line: %d)\n- ", file, line);

    printf("LOG: (file: %s, line: %d)\n- ", file, line);

	va_list args;
    va_start(args, fmt);
   	vfprintf(stdout, fmt, args);
    va_end(args);

    printf("\n");
}

void BaseLogPrint(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}