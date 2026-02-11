#ifndef PRODG_SHIM_STDLIB_H
#define PRODG_SHIM_STDLIB_H

#include <stddef.h>

void* malloc(size_t size);
void free(void* ptr);
int abs(int n);

#endif
