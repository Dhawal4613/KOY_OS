#ifndef KOY_STDLIB_H
#define KOY_STDLIB_H

//#define NULL ((void *)0)
#define KOY_HEAP_SIZE (1024 * 1024)

static unsigned char heap[KOY_HEAP_SIZE];

static char *heap_start;
static char *heap_end;
static char *heap_curr;

#include <stddef.h>

void koy_heap_init(void *start, size_t size);
void *koy_kmalloc(size_t size);
void koy_kfree(void* ptr);

#endif