#ifndef KOY_STRING_H
#define KOY_STRING_H
#include<stddef.h>

void *koy_memcpy(char* dest, const char* src, size_t nb);
void *koy_memmove(char* dest, const char* src, size_t nb);
void *koy_memset(void* start, int chr, size_t end);
int koy_memcmp(const void* ptr1, const void* ptr2, size_t nb);
void *koy_memchr(const void* ptr, int value, size_t nb);

char *koy_strcpy(char* dest, const char* src);
char *koy_strncpy(char* dest, const char* src, size_t nb);
char *koy_strcat(char* dest, const char* src);
char *koy_strncat(char* dest, const char* src, size_t nb);
size_t koy_strlen(const char* str);

int koy_strcmp(const char* str1, const char* str2);
int koy_strncmp(const char* str1, const char* str2, size_t nb);
#endif
