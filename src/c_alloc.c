#include "FreeRTOS.h"
#include <stdlib.h>
#include <string.h> // memcpy


void* malloc(size_t size) { 
    return pvPortMalloc(size); 
}

void* calloc(size_t nItems, size_t itemSize) { 
    return pvPortMalloc(nItems * itemSize); 
}

void* realloc(void* ptr, size_t newSize) {
    void* res = malloc(newSize);
    if (res && ptr) {
        memcpy(res, ptr, newSize);
        free(ptr);
    }
    return res;
}

void free(void* ptr) { 
    vPortFree(ptr); 
}
