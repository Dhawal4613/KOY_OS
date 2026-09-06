#include "koy_stdlib.h"

void koy_heap_init(void *start, size_t size) {
    heap_start = start;
    heap_end = (char *)start + size;
    heap_curr = start;
}

void *koy_kmalloc(size_t size) {
    size = (size + 15) & ~15;

    if (heap_curr + size > heap_end) {
        return NULL;
    }

    void *ptr = heap_curr;
    heap_curr += size;
    return ptr;
}

void koy_kfree(void *ptr) {
    (void)ptr;
}