#ifndef PRODG_SHIM_STDIO_H
#define PRODG_SHIM_STDIO_H

#include <stdarg.h>
#include <stddef.h>

typedef struct FILE FILE;
extern FILE* stdout;
extern FILE* stderr;

int printf(const char* fmt, ...);
int sprintf(char* dst, const char* fmt, ...);
int snprintf(char* dst, size_t n, const char* fmt, ...);
int vprintf(const char* fmt, va_list ap);
int vsnprintf(char* dst, size_t n, const char* fmt, va_list ap);

#endif
